// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "McpEventHandler.h"
#include "McpConnection.h"
#include "McpConsumer.h"
#include "McpMessage.h"
#include "McpDevice.h"
#include "Parameters.h"

#include "miro/Exception.h"

#undef DEBUG

using std::cout;
using std::cerr;

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Mcp
{
  EventHandler::EventHandler(Connection& _connection, 
			     Consumer* _consumer,
			     const Parameters& _parameters) :
    Super(_consumer, new Message()),
    connection(_connection),
    watchdogTimeout(_parameters.watchdogTimeout.sec() * TIME_UNITS_PER_SECOND),
    state(NO_STARTS),
    buffLen(0),
    buffPos(0),
    writePtr((char*)message_)
  {
  }

  EventHandler::~EventHandler()
  {
    DBG(cout << "Destructing McpEventHandler." << endl);
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd) 
  {
    int bytes = 0;
    char thisChar;

    // read data from the mcp device if we do not have some left
    bytes = ACE_OS::read(fd, buffer, BUFFER_SIZE);

    if (bytes == -1)
      throw Miro::CException(errno, "Error on MCP file descriptor read.");

    if (bytes == 0)
      throw Miro::Exception("MCP file descriptor was called to read 0" \
			    "bytes from the device. I can´t belief this!");

    buffLen = bytes;
    buffPos = 0;

    DBG(cerr << "Read " << bytes << " bytes from mcp" << endl);

    // while there ist data left to parse
    while ((buffPos < buffLen)) {
      thisChar = buffer[buffPos++];
      DBG(cerr << "Working char " << buffPos 
	  << " (0x" <<  hex << (unsigned int)thisChar << dec << ")" 
	  << endl);
    
      // translate the mcp message read from fd
      // into an mcp message string:

      // each message starts with START START (0x0202)
      // each message ends with END END (0x0303)
      // a 0x02 is therefore coded as 0x0200
      // a 0x03 is therefore coded as 0x0300

      // message parsing works as follows:
      // identify the beginning and ending of a mcp message,
      // copy the bytes inbetween into a McpMessage buffer,
      // hand the McpMessage on to further processing to another Consumer
      switch(state) {
      case NO_STARTS:
	if (thisChar == START) {
	  state = ONE_START;
	}
	else {
	  cerr << "Unexpected char " << hex << (int)thisChar << dec << " before start" << endl;
	}
	break;
      case ONE_START:
	if (thisChar == START) {
	  state = IN_PROGRESS;

	  *((ACE_Time_Value*)(writePtr)) = ACE_OS::gettimeofday();
	  writePtr += sizeof(ACE_Time_Value); // Move beyond timestamp
	}
	else {
	  state = NO_STARTS; /* starts only count if in a pair */
	  cerr << "Unexpected char " << hex << (int)thisChar << dec  << " before start" << endl;
	}
	break;
      case IN_PROGRESS:
	switch(thisChar) {
	case END:
	  state = END_IN_STREAM;
	  break;
	
	case START:
	  state = START_IN_STREAM;
	  break;
	default:
	  *writePtr = thisChar;     // Copy character into message block
	  writePtr += sizeof(char); // Move beyond the inserted char
	  break;
	}
	break;
      case START_IN_STREAM:
	switch (thisChar) {
	case START:
	  // somebody screwed the pooch, because we just got 2 starts
	  // maybe we missed the last end 
	  cerr << "Unexpected start in base packet" << endl;
	  // reset base output parsing
	  state = NO_STARTS;
	  writePtr = (char*)message_;
	  break;
	case END:
	default:
	  *writePtr = START;        // since we didn't store the previous START, we do it now
	  writePtr += sizeof(char); // Move beyond the inserted char
	  state = IN_PROGRESS;
	}
	break;
      case END_IN_STREAM:
	switch (thisChar) {
	case END:
	  // get the message on its way
	  dispatchMessage();
	  writePtr = (char*)message_;
	  // reset base output parsing
	  state = NO_STARTS;
	  break;
	case START:
	default:
	  *writePtr = END;          // since we didn't store the previous END, we do it now
	  writePtr += sizeof(char); // Move beyond the inserted char
	  state = IN_PROGRESS;
	}
	break;
      }
    }
    DBG(cout << "mcpEventHandler: Done with select" << endl);

    return 0;
  }

  // timer callback
  int
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *arg) 
  {
    switch ((const int)arg) {
    case STARTUP:
      startUp();
      break;
    case WATCHDOG:
      watchdogCancel();
      break;
    default: 
      throw Miro::Exception("mcpEventHandler: Unknown timeout type.");
    }
    return 0;
  }

  //-----------------------------------------------------------------------------
  // private methods
  //-----------------------------------------------------------------------------

  // If the user tries to start status reports before we are ready to
  // handle them, we defer and start them here.  This is called via 
  // a ACE timeout when we are ready to rock
  void
  EventHandler::startUp()
  {
    DBG(cout << "completing mcp startup." << endl);

    connection.moduleRunning = true;

    DBG(cout << "Status report period is set to: " 
	<< connection.getStatusReportPeriod() << " (1/256 sec)" << endl);

    connection.setStatusReportFormat(connection.getStatusReportFormat());
    connection.setStatusReportPeriod(connection.getStatusReportPeriod());

//     /* set the first watch dog timeout */
//     DBG(cout << "Base: Watchdog timer is set to " 
// 	<< watchdogTimeout << "/256 seconds" << endl);

//     connection.watchdogTimer(watchdogTimeout);
  }

  void
  EventHandler::watchdogCancel()
  {
    DBG(cout << "Watchdog resetting timer: timeout." << endl);

    // this will cancel the impending timeout and set a new one
    // unless the user is insisting on doing it him/herself
    connection.watchdogTimer(watchdogTimeout);
  }
};
