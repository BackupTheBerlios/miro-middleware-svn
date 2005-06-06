// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PsosConnection.h"
#include "PsosEventHandler.h"
#include "PsosMessage.h"
#include "PsosDevice.h"
#include "Parameters.h"

#include "miro/Exception.h"
#include "miro/Synch.h"
#include "miro/Log.h"

#include "idl/ExceptionC.h"

#include <ace/Reactor.h>

namespace Psos
{
  using Miro::ACE_Exception;

  ACE_Time_Value const Connection::writeTimeOut(0, 100000);

  Connection::Connection(ACE_Reactor * _reactor, 
			 EventHandler * _eventHandler,
			 const Parameters& _parameters) :
    Super(_reactor, _eventHandler, _parameters),
    eventHandler(_eventHandler),
    writeMutex(),
    lastWrite(0,0)
  {
    MIRO_LOG_CTOR("Psos::Connection");

    ACE_Time_Value synchDelay(0, 500000);
    ACE_Time_Value synchTimeout(1, 0);
    ACE_Time_Value watchdogTimeout(1, 0);

    synchTimerId = 
      reactor_->schedule_timer(eventHandler, 
			       (void *)SYNCH_TIMER, // timer id
			       synchDelay,        // delay
			       synchTimeout);     // respawn every n sec.
    if (synchTimerId == -1)
      throw ACE_Exception(errno, "Failed to register timer for synch processing.");

    watchdogTimerId = 
      reactor_->schedule_timer(eventHandler, 
			       (void *)PING_TIMER, // timer id
			       watchdogTimeout,    // delay
			       watchdogTimeout);   // respawn
    if (watchdogTimerId == -1)
      throw ACE_Exception(errno, "Failed to register timer for ping processing.");
  }


  Connection::~Connection()
  {
    MIRO_LOG_DTOR("Psos::Connection");

    // deregister timers
    if (synchTimerId != -1)
      reactor_->cancel_timer(synchTimerId);
    if (watchdogTimerId != -1)
      reactor_->cancel_timer(watchdogTimerId);
  }

  void 
  Connection::writeMessage(const Message& message)
  {
    Miro::Guard guard(writeMutex);
    ACE_Time_Value time = ACE_OS::gettimeofday();

    ACE_Time_Value delta = time - lastWrite;
    if (delta < writeTimeOut) {
      ACE_OS::sleep(writeTimeOut - delta);
      time = ACE_OS::gettimeofday();
    }

    int rc = ioBuffer.send_n(message.buffer(), message.length() + 3);
    lastWrite = time;

    if (rc == -1)
      throw Miro::EDevIO("Error writing PSOS device.");
  }

  bool
  Connection::synched() const 
  { 
    return eventHandler->synch == 3; 
  }
  
  bool
  Connection::waitSynch(const ACE_Time_Value& maxWait) const {
    Miro::Guard guard(eventHandler->synchMutex);
    ACE_Time_Value timeout = ACE_OS::gettimeofday();
    timeout += maxWait;
    if (eventHandler->synch != 3)
      eventHandler->synchCondition.wait(&timeout); 
    return (eventHandler->synch == 3);
  }

  void
  Connection::close()
  {
    eventHandler->synchMutex.acquire();
    eventHandler->synch = 4;
    eventHandler->synchMutex.release();
      
    // if it's the Ppb stop IOpac SIP
    if (true || eventHandler->classname == "ppb") {
      const Message IO_SIP(SF_COMIOREQ, (unsigned short)0);
      writeMessage(IO_SIP);
    }

    // closes the PSOS connection
    writeMessage(MSG_COMCLOSE);
    ACE_OS::sleep(ACE_Time_Value(1));
  }
}
