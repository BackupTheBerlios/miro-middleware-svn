// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
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
  DifferentialMotionImpl::getMinMaxLRVelocity(CORBA::Long& minLTranslation,
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
  DifferentialMotionImpl::setTargetVelocity(const Miro::VelocityIDL& _velocity) 
  {
    Miro::VelocityIDL velocity(_velocity);

    CORBA::Long left;
    CORBA::Long right;

    velocity2lr(velocity, left, right);

    // TODO: adjust velocties
    double fastWheel = velocity.translation + 225 * velocity.rotation;
    if (fastWheel > 900)
      velocity.translation = (int)(900 - 225 * velocity.rotation);

    targetVelocity_ = velocity;

    left_ = left;
    right_ = right;
  }

  void
  DifferentialMotionImpl::setTargetVelocity(CORBA::Long left, CORBA::Long right) 
  {
    left_ = left;
    right_ = right;
    lr2velocity(left_, right_, targetVelocity_);
  }
};
