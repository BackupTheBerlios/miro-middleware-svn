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




