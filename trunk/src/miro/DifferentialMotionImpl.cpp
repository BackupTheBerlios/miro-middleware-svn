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
    velocity2lr(_velocity, left_, right_);

    // adjust velocties

    if (left_ < params_.minLTranslation) {
      left_ = params_.minLTranslation;
      right_ = (CORBA::Long) (params_.minLTranslation + _velocity.rotation * params_.wheelBase * .5);
    }
    else if (left_ > params_.maxLTranslation) {
      left_ = params_.maxLTranslation;
      right_ = (CORBA::Long) (params_.maxLTranslation + _velocity.rotation * params_.wheelBase * .5);
    }
    if (right_ < params_.minRTranslation) {
      left_ = (CORBA::Long) (params_.minRTranslation - _velocity.rotation * params_.wheelBase * .5);
      right_ = params_.minRTranslation;
    }
    else if (right_ > params_.maxRTranslation) {
      left_ = (CORBA::Long) (params_.maxRTranslation - _velocity.rotation * params_.wheelBase * .5);
      right_ = params_.maxRTranslation;
    }

    // recalculate the resulting velocity
    lr2velocity(left_, right_, targetVelocity_);
  }

  void
  DifferentialMotionImpl::setTargetVelocity(CORBA::Long _left, CORBA::Long _right) 
  {
    left_ = _left;
    right_ = _right;
    lr2velocity(left_, right_, targetVelocity_);
  }
};
