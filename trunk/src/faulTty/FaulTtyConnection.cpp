// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Synch.h"

#include "FaulTtyConnection.h"
#include "FaulTtyEventHandler.h"
#include "FaulTtyMessage.h"
//#include "FaulTtyDevice.h"
#include "Parameters.h"

#include <ace/Reactor.h>

#include <iostream>

// #undef DEBUG
 
#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif

using Miro::ACE_Exception;

namespace FaulTty
{
  const ACE_Time_Value Connection::writeTimeOut(0, 100000);

  Connection::Connection(ACE_Reactor * _reactor, 
			 EventHandler * _eventHandler,
			 const Parameters& _parameters) :
    Super(_reactor, _eventHandler, _parameters),
    eventHandler(_eventHandler)
    /*writeMutex(),
    lastWrite(0,0)*/
  {
    DBG(cout << "Constructing FaulTtyConnection" << endl);

    /*ACE_Time_Value synchDelay(0, 500000);
    ACE_Time_Value synchTimeout(1, 0);
    ACE_Time_Value watchdogTimeout(1, 0);*/

    /*synchTimerId =
      reactor->schedule_timer(eventHandler, 
			      (void *)SYNCH_TIMER, // timer id
				synchDelay,        // delay
				synchTimeout);     // respawn every n sec.
    if (synchTimerId == -1)
      throw ACE_Exception(errno, "Failed to register timer for synch processing.");

    watchdogTimerId = 
      reactor->schedule_timer(eventHandler, 
			      (void *)PING_TIMER, // timer id
			      watchdogTimeout,    // delay
			      watchdogTimeout);   // respawn
    if (watchdogTimerId == -1)
      throw ACE_Exception(errno, "Failed to register timer for ping processing.");*/
  }


  Connection::~Connection()
  {
    DBG(cout << "Destructing FaulTtyConnection" << endl);

    // deregister timers
    /*if (synchTimerId != -1)
      reactor->cancel_timer(synchTimerId);
    if (watchdogTimerId != -1)
      reactor->cancel_timer(watchdogTimerId);*/
  }

  void
  Connection::writeMessage(const Message& message)
  {
    //cout << "FaulTty Message: " << message.buffer_ << endl;    // for debug

    /*Miro::Guard guard(writeMutex);*/
    ACE_Time_Value time = ACE_OS::gettimeofday();
/*
    ACE_Time_Value delta = time - lastWrite;
    if (delta < writeTimeOut) {
      ACE_OS::sleep(writeTimeOut - delta);
      time = ACE_OS::gettimeofday();
    }
*/
    int rc = ioBuffer.send_n(message.buffer_, strlen(message.buffer_));
    lastWrite = time;
    //cout << "hab was geschrieben"<< endl;
    if (rc == -1)
      throw Miro::EDevIO("Error writing FaulTty device.");
  }

  void
  Connection::close()
  {
    /*eventHandler->synchMutex.acquire();
    eventHandler->synch = 4;
    eventHandler->synchMutex.release();

    // if it's the Ppb stop IOpac SIP
    if (true || eventHandler->classname == "ppb") {
      const Message IO_SIP(SF_COMIOREQ, (unsigned short)0);
      writeMessage(IO_SIP);
    }

    // closes the FaulTty connection
    writeMessage(MSG_COMCLOSE);
    ACE_OS::sleep(ACE_Time_Value(1));*/
  }
};
