// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
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
#include "miro/Exception.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Sparrow
{
  using Miro::StructuredPushSupplier;
  using Miro::OdometryImpl;
  using Miro::Exception;
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;

  MotionImpl::MotionImpl(Connection& _connection,
			 Consumer& _consumer) throw(Exception) :
    Miro::DifferentialMotionImpl(Parameters::instance()->motion),
    params_(Parameters::instance()),
    connection(_connection),
    consumer(_consumer)
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
    connection.setPower(0,0);
  }

  void 
  MotionImpl::setVelocity(const Miro::VelocityIDL& vel) throw(EOutOfBounds, EDevIO)
  {
    testVelocityBounds(vel);

    Miro::Guard guard(mutex_);
    setTargetVelocity(vel);

    connection.setSpeedRot(targetVelocity_.translation, rad2ticks(targetVelocity_.rotation));
  }

  void
  MotionImpl::setLRPower(CORBA::Long left, CORBA::Long right) throw(EOutOfBounds, EDevIO)
  {
    if (abs(left) > 30000 || abs(right) > 30000)
      throw EOutOfBounds();

    connection.setPower(left, right);
  };

  void
  MotionImpl::setLRVelocity(CORBA::Long left, CORBA::Long right)  throw(EOutOfBounds, EDevIO)
  {
    testVelocityLRBounds(left, right);

    Miro::Guard guard(mutex_);
    setTargetVelocity(left, right);
    connection.setSpeed(left_, right_);
  };
};
