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
#ifndef faulMotionImpl_hh
#define faulMotionImpl_hh

#include "idl/SparrowMotionS.h"
#include "idl/OdometryC.h"
#include "miro/DifferentialMotionImpl.h"
#include "miro/Exception.h"

// forward declerations
class ACE_Reactor;
namespace Miro
{
  class Exception;
};

namespace FaulMotor
{
  // forward declerations
  class Connection;
  class Parameters;

  class MotionImpl :  public virtual POA_Miro::SparrowMotion,
		      public virtual Miro::DifferentialMotionImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    MotionImpl(Connection& _connection) throw(Miro::Exception);
    virtual ~MotionImpl();

    //-------------------------------------------------------------------------
    // Motion interface
    //-------------------------------------------------------------------------
    void limp() throw(Miro::EDevIO);
    void setVelocity(const Miro::VelocityIDL& vel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setLRVelocity(CORBA::Long left, CORBA::Long right)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    void setLRPower(CORBA::Long, CORBA::Long)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void initMax(short maxAcc, short maxPower, short maxSpeed, double maxTurn)
     throw();

  protected:
    const Parameters *  params_;
    Connection&         connection; // encapsulating communication to hardware

  };
};

#endif




