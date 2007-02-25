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
#ifndef motionImpl_hh
#define motionImpl_hh

#include "idl/PioneerMotionS.h"
#include "idl/OdometryC.h"
#include "miro/DifferentialMotionImpl.h"
#include "miro/Exception.h"

// forward declerations
class ACE_Reactor;
namespace Miro
{
  class Exception;
};

namespace Pioneer
{
  // forward declerations
  class Connection;
  class Consumer;
  class Parameters;

  class MotionImpl :  public virtual POA_Miro::PioneerMotion,
		      public virtual ::Miro::DifferentialMotionImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    MotionImpl(Connection& _connection,
	       Miro::Odometry_ptr _pOdometry =  Miro::Odometry::_nil())
      throw(Miro::Exception);
    virtual ~MotionImpl();

    //-------------------------------------------------------------------------
    // General motion interface
    //-------------------------------------------------------------------------
    virtual void limp() 
      throw(Miro::EDevIO);
    virtual void setVelocity (const Miro::VelocityIDL & vel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    //-------------------------------------------------------------------------
    // Differential motion interface
    //-------------------------------------------------------------------------
    virtual void setLRVelocity(CORBA::Long left, CORBA::Long right) 
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // Closed motion interface
    //-------------------------------------------------------------------------
    virtual void rotateToPosition(CORBA::Double heading) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual void rotateRelative(CORBA::Double angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual void translateRelative(CORBA::Long distance)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    // protected:
    Connection&         connection; // encapsulating communication to hardware
    Miro::Odometry_var  pOdometry;  // odometry for absulute turning
    const Parameters *  params_;    // parameter struct

    static ACE_Time_Value maxWait;
  };
};

#endif




