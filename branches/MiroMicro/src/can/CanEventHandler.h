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
#ifndef CanEventHandler_h
#define CanEventHandler_h

#include "miro/DevEventHandler.h"

namespace Can
{
  // forward declarations
  class Message;
  class Parameters;

  //! Event handler for reading can messages from a device.
  /** Can messages never come fragmented and can therefore be read at once. */
  class EventHandler : public Miro::DevEventHandler
  {
    //! Super class accessor.
    typedef Miro::DevEventHandler Super;

  public:
    //! Initializing constructor.
    EventHandler(Miro::DevConsumer* _consumer,
		 Parameters const * _params);
    //private:
    virtual ~EventHandler();

    //! Inherited method.
    /** Called to handle a pending can message, when returning from select. */
    virtual int handle_input (ACE_HANDLE fd); // fd select

    //! Factory method for can messages.
    /** Needed as we support two different can drivers. */
    static Message * newMessage(Parameters const * _params);

  protected:
    Message * msg_;
    Parameters const * params_;
  };
}
#endif



