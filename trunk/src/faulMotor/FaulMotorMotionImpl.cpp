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
#include "FaulMotorMotionImpl.h"
#include "FaulMotorConnection.h"
#include "FaulMotorConsumer.h"
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

namespace FaulMotor
{
  //using Miro::StructuredPushSupplier;
  using Miro::OdometryImpl;
  using Miro::Exception;
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;

  using std::cout;
  using std::cerr;
  using std::endl;

  MotionImpl::MotionImpl(Connection& _connection) throw(Exception) :
    Miro::DifferentialMotionImpl(Parameters::instance()->motion),
    params_(Parameters::instance()),
    connection(_connection)
  {
    DBG(cout << "Constructing SparrowMotionImpl" << endl);
  }

  MotionImpl::~MotionImpl()
  {
    DBG(cout << "Destructing SparrowMotionImpl" << endl);
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
    connection.setSpeed(left_, right_);
  }

  void
  MotionImpl::setVelocity(const Miro::VelocityIDL& vel) throw(EOutOfBounds, EDevIO)
  {
    testVelocityBounds(vel);

    Miro::Guard guard(mutex_);
    setTargetVelocity(vel);

    connection.setSpeed(left_, right_);
  }

  void
  MotionImpl::setLRVelocity(CORBA::Long left, CORBA::Long right)  throw(EOutOfBounds, EDevIO)
  {
    testVelocityLRBounds(left, right);
    Miro::Guard guard(mutex_);
    setTargetVelocity(left, right);
    connection.setSpeed(left_, right_);
  };

  void
  MotionImpl::setLRPower(CORBA::Long , CORBA::Long )  throw(EOutOfBounds, EDevIO)
  {
    std::cerr << "set power not implemented" << endl;
  };

  void
  MotionImpl::initMax(short /*maxAcc*/, short /*maxPower*/, 
		      short /*maxSpeed*/, double /*maxTurn*/) throw()
  {
    std::cerr << "init max  not implemented" << endl;
  };
}
