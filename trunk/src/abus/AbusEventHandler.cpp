// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


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
    MIRO_DBG(ABUS,LL_CTOR_DTOR,"Constructing AbusEventHandler");
  }

  EventHandler::~EventHandler() {
    MIRO_DBG(ABUS,LL_CTOR_DTOR,"Destructing AbusEventHandler.");
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd) 
  {
    MIRO_DBG(ABUS,LL_PRATTLE, "abusEventHandler: handle_input");

    Message* msg = (Message*)message_;
    int count = ACE_OS::read(fd, msg->buffer(), Message::MSG_LEN);

    // since we are called by the ACE_Reactor, we dont emit exceptions
    // but just return on error. - We could return and deregister the
    // event handler (return 1), but then we also have to shut down the
    // whole server...
    if (count == 0) {
      cerr << "handle_input called with no data!" << endl;
      return 0;
    }

    if (count != Message::MSG_LEN) {
      cerr << "abusEventHandler: read() != sizeof(Message): " 
	   << count << " != " << Message::MSG_LEN << endl;
      return 0;
    }
 
    MIRO_DBG_OSTR(ABUS,LL_PRATTLE,"abusEventHandler: major Op: 0x" << hex << (int)msg->majorOp()
        << " minor op: 0x" << (int)msg->minorOp() << dec );

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
      MIRO_LOG_OSTR(LL_ERROR,"abus error: unhandled major - 0x" 
	   << std::hex << (int)msg->majorOp() << std::dec);
    }
    MIRO_DBG(ABUS,LL_PRATTLE,"Done with select");

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
      MIRO_DBG_OSTR(ABUS,LL_PRATTLE,"libmsp: LINK_REPLY or LONG_ID -"
          << " devId: 0x" <<(int)msg.devId()
          << " devStatus: 0x" << (int)message.longId().devStatus().byte
          << dec );
    }
    else {
      MIRO_DBG(ABUS,LL_PRATTLE,"LINK_REPLY or LONG_ID: No free devs available at this time.");
      connection.linkApprove(0, 1);	// approve for new devs
    }
  }

  void
  EventHandler::id(const Message&)
  {
    MIRO_LOG(LL_WARNING,"libmsp: mspM2D - unhandled major M2D_ID");
  }

  void
  EventHandler::type(const Message&)
  {
    MIRO_LOG(LL_WARNING,"libmsp: mspM2D - unhandled major M2D_TYPE");
  }

  void
  EventHandler::status(const Message&)
  {
    MIRO_LOG(LL_WARNING,"libmsp: mspM2D - unhandled major M2D_STATUS");
  }
    
  void
  EventHandler::disconnect(const Message& msg)
  {
    MIRO_DBG_OSTR(ABUS,LL_PRATTLE,"abusEventHandler: disconnect" << hex << (int)msg.devId() << dec);

    deviceTable[msg.devId()].state = NOT;
  }

  /*
   * a new device is signalling its presence
   */    
  void
  EventHandler::linkApproveAck(const Message& msg)
  {
    int devId = msg.devId();

    MIRO_DBG_OSTR(ABUS,LL_PRATTLE,"abusEventHandler: linkApproveAck: " << hex << devId << dec);

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
};
