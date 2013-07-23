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
#include "DevEventHandler.h"
#include "DevConsumer.h"
#include "DevMessage.h"
#include "Exception.h"

#include <miro/Log.h>

namespace Miro
{
  DevEventHandler::DevEventHandler(DevConsumer * _consumer,
                                   DevMessage * _message) :
      Super(),
      handle_(ACE_INVALID_HANDLE),
      consumer_(_consumer),
      message_(_message)
  {
    MIRO_DBG(MIRO, LL_CTOR_DTOR, "Constructing DevEvent.\n");
  }

  DevEventHandler::~DevEventHandler()
  {
    MIRO_DBG(MIRO, LL_CTOR_DTOR, "Destructing DevEvent.\n");

    delete consumer_;
    // FIXME: this gives a segmentation fault with can
    delete message_;
  }

  int
  DevEventHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
  {
    MIRO_DBG(MIRO, LL_NOTICE, "Device handle_close called.\n");

    // commit suicide
    delete this;
    return 0;
  }

  void
  DevEventHandler::set_handle(ACE_HANDLE _handle)
  {
    handle_ = _handle;
  }

  ACE_HANDLE
  DevEventHandler::get_handle() const
  {
    return handle_;
  }

  void
  DevEventHandler::dispatchMessage()
  {
    consumer_->handleMessage(message_);
  }
}
