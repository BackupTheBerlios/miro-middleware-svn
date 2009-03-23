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
#include "PioneerMotionImpl.h"
#include "PioneerConnection.h"
#include "PioneerConsumer.h"
#include "Parameters.h"

#include "miro/Synch.h"
#include "miro/Exception.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif


namespace Pioneer
{
  using std::cout;
  using std::cerr;
  using std::endl;

  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;
  using Miro::Exception;

  // maximum wait time for cond.wait calls
  ACE_Time_Value MotionImpl::maxWait = ACE_Time_Value(0, 500000);

  MotionImpl::MotionImpl(Connection& _connection,
			 Miro::Odometry_ptr _pOdometry) throw(Exception) :
    Miro::DifferentialMotionImpl(Parameters::instance()->motion),
    connection(_connection),
    pOdometry(Miro::Odometry::_duplicate(_pOdometry)),
    params_(Parameters::instance())
  {
   DBG(cout << "Constructing PioneerMotionImpl" << endl);
  }

  MotionImpl::~MotionImpl()
  {
   DBG(cout << "Destructing PioneerMotionImpl" << endl);
  }

  // 
  // IDL interface implementation

  //--------------------------------------------------------------------------
  // General motion interface implementation
  //--------------------------------------------------------------------------

  void 
  MotionImpl::limp() throw(EDevIO)
  {
    Miro::VelocityIDL v;
    v.translation = 0;
    v.rotation = 0.;

    Miro::Guard guard(mutex_);
    setTargetVelocity(v);
    connection.setSpeedRot(0,0);
  }

  void 
  MotionImpl::setVelocity(const Miro::VelocityIDL& vel) throw(EOutOfBounds, EDevIO)
  {
    testVelocityBounds(vel);

    Miro::Guard guard(mutex_);
    setTargetVelocity(vel);
    connection.setSpeedRot((short)targetVelocity_.translation,
			   (short)Miro::rad2Deg(targetVelocity_.rotation));
  }

  //--------------------------------------------------------------------------
  // Differential motion interface implementation
  //--------------------------------------------------------------------------


  void 
  MotionImpl::setLRVelocity(CORBA::Long left, CORBA::Long right)
    throw(EDevIO)
  {
    testVelocityLRBounds(left, right);

    Miro::Guard guard(mutex_);
    setTargetVelocity(left, right);
    connection.setSpeed2((short)left_, (short)right_);
  }
  
  //--------------------------------------------------------------------------
  // Closed motion interface implementation
  //--------------------------------------------------------------------------

  void 
  MotionImpl::rotateToPosition(CORBA::Double heading)
    throw(EOutOfBounds, EDevIO)
  {
    if (!CORBA::is_nil(pOdometry.in())) {
      Miro::PositionIDL position = pOdometry->getPosition();
      Miro::Angle angle(position.heading - heading);

      rotateRelative(angle.rad());
    }
  }
  
  void 
  MotionImpl::rotateRelative(CORBA::Double relative)
    throw(EOutOfBounds, EDevIO)
  {
    Miro::VelocityIDL v;
    v.translation = 0;
    v.rotation = 0.;

    Miro::Guard guard(mutex_);
    setTargetVelocity(v);
    connection.turn((short)Miro::rad2Deg(relative));
  }

  void MotionImpl::translateRelative(CORBA::Long distance)
    throw(EOutOfBounds, EDevIO)
  {
    if (distance > 5000)
      throw EOutOfBounds();
    connection.translate((short) distance);
  }
  
};
