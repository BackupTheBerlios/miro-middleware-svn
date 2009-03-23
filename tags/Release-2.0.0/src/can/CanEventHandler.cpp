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
#include <ace/Message_Block.h>

#include "CanEventHandler.h"
#include "CanMessage.h"
#include "PCanMessage.h"
#include "Can0Message.h"
#include "Parameters.h"
#include "pcan.h"

#include "miro/TimeHelper.h"
#include "miro/Log.h"

namespace Can
{
  int canCounter = 0;

  Message * EventHandler::newMessage(Parameters const * _params)
  {
     Message * msg;
     if(_params->module == "pcan") {
        msg = new PCanMessage();
     }
     else{
        msg = new Can0Message();
     }
     return msg;
  }

  EventHandler::EventHandler(Miro::DevConsumer * _consumer, 
			     Parameters const * _params) :
    Super(_consumer, newMessage(_params)),
    msg_(static_cast<Message *>(message_)),
    params_(_params)
  {
    MIRO_LOG_CTOR("Can::EventHandler");
  }

  EventHandler::~EventHandler()
  {
    MIRO_LOG_DTOR("Can::EventHandler");
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd)
  {
    MIRO_DBG(SPARROW, LL_PRATTLE, "Can::EventHandler: handle_input()");

    int count;
    if(params_->module == "pcan"){
       pcanmsg * msgp;
       msg_->canMessage((int **) &msgp);
       count = ioctl(fd, PCAN_READ_MSG, msgp);
       if (count < 0) {
         MIRO_LOG(LL_ERROR, "Pcan handle_input() failed!");
         return 0;
       }
    }
    else {
       canmsg * msg;
       msg_->canMessage((int **) &msg);
       count = ACE_OS::read(fd, msg, sizeof(canmsg));

      if (count == 0) {
        MIRO_LOG(LL_WARNING,
                 "Can::EventHandler: handle_input() called with no data!");
       return 0;
      }

       if (count != sizeof(canmsg)) {
         MIRO_LOG_OSTR(LL_ERROR,
		       "Can::EventHandler: read() != sizeof(canmsg): " <<
		       count << " != " << sizeof(canmsg));
         return 0;
       }
    }

    msg_->time() = ACE_OS::gettimeofday(); // set time stamp
    dispatchMessage();

    return 0;
  }
}
