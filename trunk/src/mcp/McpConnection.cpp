// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "idl/ExceptionC.h"
#include "miro/Exception.h"

#include "McpConnection.h"
#include "McpEventHandler.h"
#include "McpDevice.h"
#include "Parameters.h"

#include <ace/Reactor.h>

#include <iostream>


#undef DEBUG


#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Mcp
{
  using std::cout;
  using std::string;
  using Miro::Exception;
  using Miro::CException;
  using Miro::ACE_Exception;

  using Miro::EDevIO;

  ACE_Time_Value Connection::writeTimeOut(0, 10000);

  //
  // Constructors / Destructors
  //

  Connection::Connection(ACE_Reactor* _reactor,
			 EventHandler* _eventHandler,
			 const Parameters& _parameters)
    throw(Exception, EDevIO):
    Super(_reactor, _eventHandler, _parameters),
    eventHandler(_eventHandler),
    watchdogTimerId(-1),
    moduleRunning(false),
    comMutex(),
    comCond(comMutex),
    comBlocked(false),
    lastWrite(0),
    reportFormat(0),
    reportPeriod(0)
  {
    DBG(cout << "Constructing McpConnection." << endl);

    // In case base is hosed. SIGALRM will be thrown. If we catch it,
    // MCP did not respond in time.  If not, cancel alarm 
    // Since the reactor isn't running yet, we have to do this native
    ACE_OS::signal(SIGALRM, &deadHandler); 
    ACE_OS::alarm(INITIAL_WAIT_TIME);

    // will definitely choke if base is off
    setBinaryMode();

    // if we made it back, cancel alarm 
    ACE_OS::alarm(0);
    ACE_OS::signal(SIGALRM, SIG_DFL);

    // reset communication
    synch();

    //  resetComm();

    // register handlers at the reactor

    // we want to reset the watchdog interval with a margin 
    // of safety.  If we miss, the mcp will go limp. 
    // So, we reset at some fraction of the timeout time
//     ACE_Time_Value watchdogTimeout;
//     watchdogTimeout.msec(_parameters.watchdogTimeout.msec() / 2);
    
//     if (watchdogTimeout != ACE_Time_Value(0)) {
//       DBG(cout << "Watchdog timer interval: " << watchdogTimeout << endl);

//       watchdogTimerId = 
// 	reactor->schedule_timer(eventHandler, 
// 				(void *)WATCHDOG, // timer id
// 				watchdogTimeout,  // delay
// 				watchdogTimeout); // respawn every timeout sec.
//       if (watchdogTimerId == -1)
// 	throw ACE_Exception(errno, "Failed to register timer for watchdog canceling.");
//     }

    // Complete initialization after entering of the reactor processing loop.
    // So we start immediately after the start of the reactor
    ACE_Time_Value startReports(0, 1); 
    if (reactor->schedule_timer(eventHandler, 
				(void *)STARTUP, // timer id
				startReports)    // delay
	== -1)
      throw ACE_Exception(errno, "Failed to register timer for status report startup.");
  }

  Connection::~Connection()
  {
    DBG(cout << "Destructing Mcp" << endl);

    // Stop watchdog timer
    // - otherwise the base will lock itself
    if (watchdogTimerId != -1)
      reactor->cancel_timer(watchdogTimerId);
    watchdogTimer(0);

    // Stop hardware triggered communication
    setStatusReportPeriod(0);
    moduleRunning = false;

    // reset base to ascii mode
    leaveDirectMode();
  }

  //---------------------------------------------------------------------------
  // Status Report Calls
  //---------------------------------------------------------------------------
  void 
  Connection::setStatusReportFormat(unsigned int data)
  {
    reportFormat = data;

    if (moduleRunning)
      setStatusData(reportFormat);
  }

  void 
  Connection::setStatusReportPeriod(unsigned int period)
  {
    reportPeriod = period;

    // do not actually start reports coming unless this is called
    // after base reactor is running
    if (moduleRunning && reportFormat != 0) {
      setStatusData(reportFormat);
      setStatusPeriod(reportPeriod);
    }
  }

  void 
  Connection::sendCmd(const OutMessage* message) const 
  {
    DBG(cout << "Sending message to mcp. opCode 0x" 
	<< hex << (int)(message->opcode()) 
	<< " - prameter 0x" << message->parameter() << dec << endl);

    // Miro::Guard guard(comMutex);
    if (ioBuffer.send_n(message, sizeof(OutMessage)) == -1)
      throw EDevIO();
  }

  void
  Connection::sendCommands(const iovec *vector, size_t count)
  {
#ifdef DEBUG
    cout << "Sending messages to mcp." << endl;
    for (unsigned int i=0; i < count; ++i)
      cout << "  opcode 0x" << hex 
           << (int)((OutMessage*)(vector[i].iov_base))->opcode()
           << " - parameter 0x"
           << ((OutMessage*)(vector[i].iov_base))->parameter()
           << endl;
#endif

#ifdef USER_MSG_SYNCH
    ACE_Time_Value timeout = ACE_OS::gettimeofday();
    timeout += ACE_Time_Value(0, 500000);
    Miro::Guard guard(comMutex);
    if (((OutMessage*)
    	 (vector[count-1].iov_base))->opcode() ==  OP_USER_MESSAGE) {
      while (comBlocked)
	if (comCond.wait(&timeout) == -1) {
	  cout << "McpConnection::sendCommands - timeout" << endl;
	  break;
	}
      comBlocked = true;
    }
#endif
    
    Miro::Guard guard(comMutex);
    ACE_Time_Value time = ACE_OS::gettimeofday();
    ACE_Time_Value delta = time - lastWrite;
    if (delta < writeTimeOut) {
      ACE_OS::sleep(writeTimeOut - delta);
      time = ACE_OS::gettimeofday();
    }

    if (((OutMessage*)
    	 (vector[count-1].iov_base))->opcode() ==  OP_USER_MESSAGE)
      --count;

    int rc = ioBuffer.send(vector, count);
    lastWrite = time;

    if (rc == -1)
      throw EDevIO("Error writing Mcp device.");
  }

  // this should throw an exception on error
  void
  Connection::setBinaryMode() const throw(CException)
  {
    static const char* message = "\rBE FFFF\rDM\r";
    unsigned char buffer[20]; 
    int rc = ioBuffer.send_n(message, strlen(message));

    // sleep for a quarter second to let any ASCII commands the MCP is 
    // processing to end
    ACE_OS::sleep(ACE_Time_Value(0, 250000));

    // clear out any crap that might have been left over from ASCII mode
    do {
      if (rc == -1)
	throw CException(errno, "mcp.setBinaryMode()");
    }
    while((rc = ioBuffer.recv(buffer, 20)));
  }

  void 
  Connection::deadHandler(int)
  {
    static string errorMessage = "Motor controller did not respond." \
      "\nPerhaps the base/arm needs to be reset.";
    throw Miro::Exception(errorMessage);
  }
}
