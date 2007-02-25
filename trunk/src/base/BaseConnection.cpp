// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include <ace/Reactor.h>

#include "BaseConnection.h"
#include "BaseEventHandler.h"
#include "BaseConsumer.h"
#include "Parameters.h"

#include "miro/Exception.h"

#include <iostream>


namespace Base
{
  Connection::Connection(ACE_Reactor * _reactor, 
			 Mcp::EventHandler * _mcpEventHandler,
			 Consumer * _consumer,
			 EventHandler * _eventHandler,
			 const Parameters& _parameters)
    throw(Miro::Exception, Miro::EDevIO) :
    Super(_reactor, _mcpEventHandler, _parameters),
    rotateState(LIMP),
    rotateVelocity(0x0080),
    rotateAcceleration(0x0400),
    translateState(LIMP),
    translateVelocity(0x800),
    translateAcceleration(0x4000),
    stateMutex(),
    eventHandler(_eventHandler),
    baseConsumer(_consumer),
    batteryTimerId(-1)
  {
    // set base rotation and speed to zero and initiate an index report
    const iovec vec[3] = { { &MSG_ROT_LIMP, sizeof(Mcp::OutMessage)}, 
			   { &MSG_TRANS_LIMP, sizeof(Mcp::OutMessage)},
			   { &MSG_INDEX_REPORT, sizeof(OutMessage)} };
    sendCommands(vec, 3);

    // trigger polling of the current battery voltage
    ACE_Time_Value batteryStart(BATTERYPOLL_START);
    ACE_Time_Value batteryPolling(BATTERYPOLL_INTERVAL);
    
    batteryTimerId = reactor_->schedule_timer(eventHandler, 
					     NULL,            // no parameter
					     batteryStart,    // delay
					     batteryPolling); // respawn every timeout sec.
    if (batteryTimerId == -1)
      throw Miro::ACE_Exception(errno, "failed to register battery poll timer");
  }

  Connection::~Connection() 
  {
    // halt robot
    const iovec vec[2] = { {&MSG_ROT_LIMP, sizeof(OutMessage)}, 
			   {&MSG_TRANS_LIMP, sizeof(OutMessage)} };
    sendCommands(vec, 2);

    // stop battery polling
    reactor_->cancel_timer(batteryTimerId);
    
    // deleting eventHandler handler
    delete eventHandler;
  }

  void
  Connection::setTranslateVelocity(unsigned long arg) throw(Miro::EDevIO)
  {
    Miro::Guard guard(stateMutex);

    if (translateVelocity != arg) {
      translateVelocity = arg;
#ifdef DEBUG
      std::cerr << __FILE__ << ":"
	   << __LINE__ << ":"
	   << __FUNCTION__ << "() - " 
	   << arg << ": 0x"
	   << std::hex << convertMMToEncoders(arg) << std::dec << std::endl;
#endif
      OutMessage message(OP_TRANS_VEL, convertMMToEncoders(arg));
      const iovec vec[2] = { {&message, sizeof(OutMessage)}, 
                             {&MSG_USER_MESSAGE, sizeof(OutMessage)} };
      sendCommands(vec, 2);	
    }
  }

}
