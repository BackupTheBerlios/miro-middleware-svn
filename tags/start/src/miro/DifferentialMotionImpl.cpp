// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "DifferentialMotionImpl.h"

namespace Miro
{
  DifferentialMotionImpl::DifferentialMotionImpl(const DifferentialMotionParameters& _params) :
    MotionImpl(_params),
    params_(_params)
  {
    left_ = 0;
    right_ = 0;
  }

  void
  DifferentialMotionImpl::getMinMaxLRVeclocity(CORBA::Long& minLTranslation,
					       CORBA::Long& maxLTranslation,
					       CORBA::Long& minRTranslation,
					       CORBA::Long& maxRTranslation) throw()
  {
    minLTranslation = params_.minLTranslation;
    maxLTranslation = params_.maxLTranslation;
    minRTranslation = params_.minRTranslation;
    maxRTranslation = params_.maxRTranslation;
  }

  void
  DifferentialMotionImpl::getTargetLRVelocity(CORBA::Long& left,
					      CORBA::Long& right) throw()
  {
    Guard guard(mutex_);

    left = left_;
    right = right_;
  }

  void
  DifferentialMotionImpl::setTargetVelocity(Miro::VelocityIDL& velocity) 
  {
    Guard guard(mutex_);

    CORBA::Long left;
    CORBA::Long right;

    velocity2lr(velocity, left, right);

    // TODO: adjust velocties
	  double fastWheel = velocity.translation + 225 * velocity.rotation;
	  if (fastWheel > 900)
	    velocity.translation = (int)(900 - 225 * velocity.rotation);

    setTargetVelocity(velocity);

    left_ = left;
    right_ = right;
  }

  void
  DifferentialMotionImpl::setTargetVelocity(CORBA::Long left, CORBA::Long right) 
  {
    Guard guard(mutex_);

    Miro::VelocityIDL velocity;
    lr2velocity(left, right, velocity);
    setTargetVelocity(velocity);

    left_ = left;
    right_ = right;
  }
};
