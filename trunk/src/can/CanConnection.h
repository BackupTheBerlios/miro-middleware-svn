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
#ifndef CanConnection_h
#define CanConnection_h

// A general object oriented access class "CanConnection" for the CAN-bus
// interface.
//

#include "CanMessage.h"

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/DevConnection.h"

namespace Can
{
  // forward declerations
  class Parameters;

  class Connection : public Miro::DevConnection
  {
    typedef Miro::DevConnection Super;

  public:
    Connection(ACE_Reactor* _reactor, 
	       Miro::DevEventHandler* _eventHandler,
	       Parameters const& _parameters);

    /** write up to 8 bytes to a 29bits long id */
    void write(Message& message);

    static ACE_Time_Value canTimeOut;

  protected:
    // initialization
    void initDevice() const;

    Parameters const& parameters_;
    Miro::Mutex writeMutex;

    ACE_Time_Value lastWrite;
  };
}
#endif
