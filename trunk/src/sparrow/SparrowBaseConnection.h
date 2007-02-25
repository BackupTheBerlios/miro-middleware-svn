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
#ifndef SparrowBaseConnection_h
#define SparrowBaseConnection_h

#include "can/CanConnection.h"         // Super class

//------------------------ SparrowClass ---------------------------//

namespace Sparrow
{
  // forward declaration
  class Parameters;

  //! Base class for a connection to the sparrow hardware.
  class BaseConnection : public Can::Connection
  {
    //! Super class accessor.
    typedef Can::Connection Super;

  public:
    //! Initializing constructor.
    BaseConnection(ACE_Reactor * _reactor,
		   Miro::DevEventHandler * _eventHandler);
    //! Cleaning up.
    virtual ~BaseConnection();
    //! Init method to be called once by the event handler.
    virtual void init() = 0;
    virtual void fini() = 0;

  protected:
    //! Pointer to the parameters.
    Parameters const * const params_;
  };
}

#endif // SparrowBaseConnection_h

