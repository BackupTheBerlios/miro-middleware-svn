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
			 Consumer& _consumer,
			 Miro::Odometry_ptr _pOdometry) throw(Exception) :
    Miro::DifferentialMotionImpl(Parameters::instance()->motion),
    connection(_connection),
    consumer(_consumer),
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
