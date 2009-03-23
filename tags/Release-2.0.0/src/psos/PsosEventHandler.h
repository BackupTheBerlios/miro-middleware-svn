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
#ifndef PsosEventHandler_h
#define PsosEventHandler_h

#include "miro/DevEventHandler.h"
#include "miro/Synch.h"

#include <string>

namespace Psos
{
  // forward declarations
  class Message;
  class Connection;
  class Parameters;

  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;

    // psos event handling defines
    // states of psos io packet
    typedef enum { NO_STARTS, START_ONE, GET_LENGTH, IN_PROGRESS } state_type;

  public:
    EventHandler(Miro::DevConsumer* _consumer, 
		 Connection& _connection,
		 Parameters const * _params);
    // private:
    virtual ~EventHandler();

    virtual int handle_input (ACE_HANDLE fd); // fd select
    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  protected:
    virtual void dispatchMessage();

    void parseSynchMessage(const Message& _message);
    void initDevice();

    Connection& connection;
    Parameters const * params_;

  public:    
    Miro::Mutex     synchMutex;
    Miro::Condition synchCondition;
    int synch;

    // status variables for processing of
    // data coming asychronously form the mcp
    state_type state;
    unsigned char buff[512];
    int  buffLen;
    int  buffPos;

    unsigned char * writePtr;

    std::string name;
    std::string classname;
    std::string subclass;
  };
};
#endif



