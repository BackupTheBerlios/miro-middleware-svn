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
#ifndef AbusEventHandler_h
#define AbusEventHandler_h

#include "miro/DevEventHandler.h"

namespace Abus
{
  // forward declerations
  class Connection;
  class Message;

  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;

  public:
    EventHandler(Connection& connection_, 
		 Miro::DevConsumer* consumer_,
		 Miro::DevMessage * message_);
    // private:
    virtual ~EventHandler();

    virtual int handle_input (ACE_HANDLE fd); // fd select

  protected:
    virtual void longId         (const Message& message);
    virtual void id             (const Message& message);
    virtual void type           (const Message& message);
    virtual void status         (const Message& message);
    virtual void disconnect     (const Message& message);
    virtual void linkApproveAck (const Message& message);

  private:
    Connection& connection;
  };
};

#endif




