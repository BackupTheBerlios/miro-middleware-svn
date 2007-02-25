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
#include "AbusEventHandler.h"
#include "AbusConnection.h"
#include "AbusMessage.h"
#include <miro/Log.h>

namespace Abus
{
  EventHandler::EventHandler(Connection& _connection, 
			     Miro::DevConsumer* _consumer,
			     Miro::DevMessage* _message) :
    Super(_consumer, _message),
    connection(_connection)
  {
    MIRO_LOG_CTOR("Abus::EventHandler");
  }

  EventHandler::~EventHandler() {
    MIRO_LOG_DTOR("Abus::EventHandler");
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd) 
  {
    MIRO_DBG(B21, LL_PRATTLE, "abusEventHandler: handle_input");

    Message* msg = (Message*)message_;
    int count = ACE_OS::read(fd, msg->buffer(), Message::MSG_LEN);

    // since we are called by the ACE_Reactor, we dont emit exceptions
    // but just return on error. - We could return and deregister the
    // event handler (return 1), but then we also have to shut down the
    // whole server...
    if (count == 0) {
      MIRO_LOG(LL_WARNING, "Abus::EventHandler::handle_input() called with no data!" );
      return 0;
    }

    if (count != Message::MSG_LEN) {
      MIRO_LOG_OSTR(LL_WARNING, 
		    "Abus::EventHandler::handle_input: read() != sizeof(Message): " <<
		    count << " != " << Message::MSG_LEN);
      return 0;
    }
 
    MIRO_DBG_OSTR(B21, LL_PRATTLE,
		  "abusEventHandler: major Op: 0x" << std::hex << 
		  (int)msg->majorOp() << 
		  " minor op: 0x" << (int)msg->minorOp() << std::dec );

    if (deviceTable[msg->devId()].state == NOT)
      deviceTable[msg->devId()].state = NEW;   // first message of device

    switch (msg->majorOp()) {
    case M2D_LINK_REPLY:       
      longId(*msg);
      break;
    case M2D_LONG_ID: 
      longId(*msg);
      break;
    case M2D_ID:              
      id(*msg);
      break;
    case M2D_TYPE: 
      type(*msg);
      break;
    case M2D_STATUS: 
      status(*msg);
      break;
    case M2D_MSG_TO_DRIVER:
      msg->time() = ACE_OS::gettimeofday(); // set time stamp
      dispatchMessage();
      break;
    case M2D_DISCONNECT:
      disconnect(*msg);
      break;
    case M2D_LINK_APPROVE_ACK: 
      linkApproveAck(*msg);
      break;
    default:
      MIRO_LOG_OSTR(LL_ERROR, 
		    "abus error: unhandled major - 0x" <<
		    std::hex << (int)msg->majorOp() << std::dec);
    }
    MIRO_DBG(B21, LL_PRATTLE, "Done with select");

    return 0;
  }

  //-----------------------------------------------------------------------------
  // protected
  //-----------------------------------------------------------------------------
  void
  EventHandler::longId(const Message& msg)
  {
    const LongIdMessage& message = (const LongIdMessage&) msg;

    if (msg.minorOp() != 0xFF) {		// if device available
      // copy vital stats into deviceTable structure
      deviceTable[msg.devId()].longId = message.longId();

      if (deviceTable[msg.devId()].state != READY)
	connection.linkApprove (msg.devId(), 1);     // reply with a link approval

      // display vital stats
      MIRO_DBG_OSTR(B21, LL_PRATTLE, 
		    "libmsp: LINK_REPLY or LONG_ID -"
		    << " devId: 0x" <<(int)msg.devId()
		    << " devStatus: 0x" << (int)message.longId().devStatus().byte
		    << std::dec );
    }
    else {
      MIRO_DBG(B21, LL_PRATTLE, 
	       "LINK_REPLY or LONG_ID: No free devs available at this time.");
      connection.linkApprove(0, 1);	// approve for new devs
    }
  }

  void
  EventHandler::id(const Message&)
  {
    MIRO_LOG(LL_WARNING, "libmsp: mspM2D - unhandled major M2D_ID");
  }

  void
  EventHandler::type(const Message&)
  {
    MIRO_LOG(LL_WARNING, "libmsp: mspM2D - unhandled major M2D_TYPE");
  }

  void
  EventHandler::status(const Message&)
  {
    MIRO_LOG(LL_WARNING, "libmsp: mspM2D - unhandled major M2D_STATUS");
  }
    
  void
  EventHandler::disconnect(const Message& msg)
  {
    MIRO_DBG_OSTR(B21, LL_PRATTLE,
		  "abusEventHandler: disconnect" << 
		  std::hex << (int)msg.devId() << std::dec);

    deviceTable[msg.devId()].state = NOT;
  }

  /*
   * a new device is signalling its presence
   */    
  void
  EventHandler::linkApproveAck(const Message& msg)
  {
    int devId = msg.devId();

    MIRO_DBG_OSTR(B21, LL_PRATTLE,
		  "abusEventHandler: linkApproveAck: " << 
		  std::hex << devId << std::dec);

    if (msg.minorOp() == 0) { // connect
      deviceTable[devId].state = READY;
      deviceTable[devId].longId.devStatus().byte |= AB_STATUS_LINKED;

      connection.enable(devId, 1); // enable device
      connection.linkRequest("RWI-DEV", "MSP", "1"); // ask for next device
    }
    else {                   // disconnect
      deviceTable[devId].state = NOT;
    } 
  }
}
