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
#include "PanTiltEventHandler.h"
#include "PanTiltConnection.h"
#include "PanTiltConsumer.h"
#include "PanTiltMessage.h"

#include "miro/Log.h"

#include <iostream>

namespace DpPanTilt
{
  //
  // EventHandler is an object wrapper for the event handler 
  // registered with the reactor
  //
  EventHandler::EventHandler(Connection& _connection, Consumer * _consumer) :
    Super(_consumer, new Message()),
    connection_(_connection),
    msg_(static_cast<Message *>(message_))
  {
    state = WFSTAR;
  }

  //
  // Destructor
  //
  EventHandler::~EventHandler() 
  {	
  }

  //
  // file descriptor callback
  //
  int 
  EventHandler::handle_input (ACE_HANDLE fd) 
  {
    unsigned char thisChar;
  
    // buffer for bytes send form the panTilt    
    unsigned char buff[BUFFSIZE];
    int buffPos = 0;
    int buffLen = 0;

    buffLen = read(fd,buff,BUFFSIZE);

    if (buffLen < -1) {
      throw Miro::Exception("EventHandler::handle_input: Error on  file descriptor");
    }   else if (buffLen == 0) {    
      throw Miro::Exception("EventHandler::handle_input:  fd was called, but 0 bytes for reading");
    } else {
      buffPos = 0;
    } 

    while (buffPos < buffLen) {
      thisChar = buff[ buffPos++ ];

      // cerr << "[" << (unsigned int)(thisChar) << "]" << flush;

      switch(state) {
      case WFSTAR:
	if (thisChar == '*') {
	  state = WFDELIM;
	  msg_->time() = ACE_OS::gettimeofday(); // set time stamp
	}
	else if (thisChar == '!') {
	  state = WFDELIM;
	  msg_->setErrorState();
	}
	else {
	  MIRO_LOG_OSTR(LL_ERROR, "ignoring :|" << thisChar << "|");
	}
	break;

      case WFDELIM: 
	if (thisChar == '\n') {
	  state = WFSTAR;
	  dispatchMessage();
	  msg_->resetMessage();
	}
	else {
	  //cerr << "using :|" << thisChar << "|" << endl;	  
	  // TODO here append to message
	  msg_->addChar( thisChar );
	}
	break;
      default:
	throw Miro::Exception("EventHandler::handle_input: unknown state in panTiltEvent.");
	break;
      }
    }
    return 0;
  }

  // startup event
  int
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *)
  {
    connection_.checkConnection();
    
    Message p1("ED"); // echo disable
    connection_.writeMessage(p1);
    Message p2("FV"); // verbose feedback
    connection_.writeMessage(p2);

    return 0;
  }
};
