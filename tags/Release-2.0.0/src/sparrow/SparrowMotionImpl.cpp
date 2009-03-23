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
#include "SparrowMotionImpl.h"
#include "SparrowConnection.h"
#include "SparrowConsumer.h"
#include "Parameters.h"

#include "miro/Synch.h"
#include "miro/Log.h"

namespace Sparrow
{
  using Miro::StructuredPushSupplier;
  using Miro::OdometryImpl;
  using Miro::Exception;
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;

  MotionImpl::MotionImpl(Connection& _connection) :
    Miro::DifferentialMotionImpl(Parameters::instance()->motion),
    params_(Parameters::instance()),
    connection(_connection)
  {
    MIRO_LOG_CTOR("Sparrow::MotionImpl");
  }

  MotionImpl::~MotionImpl()
  {
    MIRO_LOG_DTOR("Sparrow::MotionImpl");
  }

  //
  // IDL interface implementation

  //--------------------------------------------------------------------------
  // Motion interface implementation
  //--------------------------------------------------------------------------

  void
  MotionImpl::limp() throw(EDevIO)
  {
    Miro::VelocityIDL v;
    v.translation = 0;
    v.rotation = 0.;

    Miro::Guard guard(mutex_);
    setTargetVelocity(v);
    if (!params_->goalie)
      connection.setPower(0,0);
    else
      connection.setSpeed(0,0);
  }

  void
  MotionImpl::setVelocity(const Miro::VelocityIDL& vel) 
    throw(EOutOfBounds, EDevIO)
  {
    testVelocityBounds(vel);

    Miro::Guard guard(mutex_);
    setTargetVelocity(vel);

    connection.setSpeedRot(targetVelocity_.translation,
			   rad2ticks(targetVelocity_.rotation));
  }

  void
  MotionImpl::setLRPower(CORBA::Long left, CORBA::Long right) 
    throw(EOutOfBounds, EDevIO)
  {
    if (abs(left) > 30000 || abs(right) > 30000)
      throw EOutOfBounds();

    connection.setPower(left, right);
  }

  void
  MotionImpl::setLRVelocity(CORBA::Long left, CORBA::Long right) 
    throw(EOutOfBounds, EDevIO)
  {
    testVelocityLRBounds(left, right);

    Miro::Guard guard(mutex_);
    setTargetVelocity(left, right);
    connection.setSpeed(left_, right_);
  }

  void
  MotionImpl::initMax(short maxAcc, short maxPower,
		      short maxSpeed, double maxTurn) throw()
  {
    Miro::Guard guard(mutex_);
    connection.initMax(maxAcc, maxPower, maxSpeed, maxTurn);
  }
}
