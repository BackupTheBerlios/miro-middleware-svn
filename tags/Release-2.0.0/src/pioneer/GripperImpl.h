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
#ifndef GripperImpl_hh
#define GripperImpl_hh

#include "idl/GripperS.h"

#include "miro/Exception.h"

// forward declarations
class ACE_Reactor;

namespace Pioneer
{
  // forward declarations
  class Connection;
  class Consumer;
};

namespace Miro
{
  class Exception;

  class GripperImpl :  public virtual POA_Miro::Gripper
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    GripperImpl(Pioneer::Connection& _connection) throw(Miro::Exception);
    virtual ~GripperImpl();

    virtual void setGripPressure(short value)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual void openGrip()
      throw(Miro::EDevIO);
    virtual void closeGrip()
      throw(Miro::EDevIO);
    virtual void raiseGrip()
      throw(Miro::EDevIO);
    virtual void lowerGrip()
      throw(Miro::EDevIO);
    virtual void storeGrip()
      throw(Miro::EDevIO);
    virtual void deployGrip()
      throw(Miro::EDevIO);
    virtual void stopGrip()
      throw(Miro::EDevIO);
    virtual void moveGrip(short ms)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

  protected:
    Pioneer::Connection&         connection; // encapsulating communication to hardware
    
    static ACE_Time_Value maxWait;
  };

};

#endif




