// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "PioneerMotionImpl.h"
#include "PioneerConnection.h"
#include "PioneerConsumer.h"
#include "PioneerParameters.h"

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
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;
  using Miro::Exception;

  // maximum wait time for cond.wait calls
  ACE_Time_Value MotionImpl::maxWait = ACE_Time_Value(0, 500000);

  MotionImpl::MotionImpl(Connection& _connection,
			 Consumer& _consumer) throw(Exception) :
    Miro::DifferentialMotionImpl(Parameters::instance()->motion),
    connection(_connection),
    consumer(_consumer),
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
    connection.setSpeed2(0,0);
  }

  void 
  MotionImpl::setVelocity(const Miro::VelocityIDL& vel) throw(EOutOfBounds, EDevIO)
  {
    testVelocityBounds(vel);

    Miro::Guard guard(mutex_);
    setTargetVelocity(vel);
    connection.setSpeedRot(targetVelocity_.translation,
			   targetVelocity_.rotation);
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
    connection.setSpeed2((short)(left / params_->vel2Divisor), 
			 (short)(right / params_->vel2Divisor));
  }
  
  //--------------------------------------------------------------------------
  // Closed motion interface implementation
  //--------------------------------------------------------------------------

  void 
  MotionImpl::rotateToPosition(CORBA::Double /* heading*/)
    throw(EOutOfBounds, EDevIO)
  {
    Miro::VelocityIDL v;
    v.translation = 0;
    v.rotation = 0.;

    Miro::Guard guard(mutex_);
    setTargetVelocity(v);

    //Add your implementation here
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
    connection.turn(relative * 180 / M_PI);
  }

  void MotionImpl::translateRelative(CORBA::Long /* distance*/)
    throw(EOutOfBounds, EDevIO)
  {
    Miro::VelocityIDL v;
    v.translation = 0;
    v.rotation = 0.;

    Miro::Guard guard(mutex_);
    setTargetVelocity(v);

    //Add your implementation here
  }
  
};
