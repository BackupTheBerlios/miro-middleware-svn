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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#ifndef McpEventHandler_h
#define McpEventHandler_h

#include "miro/DevEventHandler.h"

// forward declerations
class ACE_Reactor;
namespace Miro 
{
  class Exception;
  class CException;
};

namespace Mcp
{
  // forward declerations
  class Message;
  class Connection;
  class Consumer;
  class Parameters;

  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;

    // mcp event handling defines
    // states of mcp io packet
    typedef enum { NO_STARTS, ONE_START, IN_PROGRESS, 
		   START_IN_STREAM, END_IN_STREAM } state_type;
    // values indicating beginning and end of a mcp message
    static const char START = 2;
    static const char END = 3;

  public:
    EventHandler(Connection& _mcp, Consumer* _consumer, 
		 const Parameters& _parameters);
    // private:
    virtual ~EventHandler();

    virtual int handle_input (ACE_HANDLE fd); // fd select
    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  private:
    void startUp();
    void watchdogCancel();

    // references to other objects
    Connection& connection;

    unsigned int watchdogTimeout;

    // status variables for processing of
    // data coming asychronously form the mcp
    static const int BUFFER_SIZE = 256;

    state_type state;
    char buffer[BUFFER_SIZE];
    int  buffLen;
    int  buffPos;

    unsigned char * writePtr;
  };
};
#endif




