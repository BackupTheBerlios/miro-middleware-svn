// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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