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
#include "SparrowParameters.h"

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

  ACE_Time_Value MotionImpl::maxWait;

  MotionImpl::MotionImpl(Connection& _connection,
			 Consumer& _consumer) throw(Exception) :
    Miro::DifferentialMotionImpl(Parameters::instance()->motion),
    params_(Parameters::instance()),
    connection(_connection),
    consumer(_consumer)
  {
    DBG(cout << "Constructing SparrowMotionImpl" << endl);

    // maximum wait time for cond.wait calls
    maxWait = ACE_Time_Value(0, 500000);

    // initialize the internal position
    // management of the base hardware
    // set the robot into the middle, so we have 
    // 650m to drive in each direction befor overflow
    // of the counters - I don´t think the batteries will do it that long
    //  connection.loadHeading(0);
    //  connection.loadPosition(INITIAL_XPOS, INITIAL_YPOS);

    // we should initialize server side position management
    // still work to do!

    // position.setPosition(0x8000, 0x8000);
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
    connection.setPower(0,0);
  }

  void 
  MotionImpl::setVelocity(const Miro::VelocityIDL& vel) throw(EOutOfBounds, EDevIO)
  {
    if (abs(vel.translation) > params_->maxTransVelocity ||
    	fabs(vel.rotation) > params_->maxRotVelocity)
          throw EOutOfBounds();

    connection.setSpeedRot(vel.translation, rad2ticks(vel.rotation));
  }

  void
  MotionImpl::setLRPower(CORBA::Long left, CORBA::Long right) throw()
  {
    connection.setPower(left, right);
  };

  void
  MotionImpl::setLRVelocity(CORBA::Long left, CORBA::Long right)  throw()
  {
    connection.setSpeed(left, right);
  };
};
