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


#include "MotionImpl.h"

namespace Miro
{
  MotionImpl::MotionImpl(const MotionParameters& _params) :
    params_(_params)
  {
    targetVelocity_.translation = 0;
    targetVelocity_.rotation = 0.;
  }

  void
  MotionImpl::getMinMaxVeclocity(CORBA::Long& minTranslation,
				 CORBA::Long& maxTranslation,
				 double& minRotation,
				 double& maxRotation) throw()
  {
    minTranslation = params_.minTranslation;
    maxTranslation = params_.maxTranslation;
    minRotation = params_.minRotation;
    maxRotation = params_.maxRotation;
  }

  VelocityIDL
  MotionImpl::getTargetVelocity() throw()
  {
    Guard guard(mutex_);
    return targetVelocity_;
  }

  void
  MotionImpl::setTargetVelocity(VelocityIDL& _velocity) 
  {
    Guard guard(mutex_);
    targetVelocity_ = _velocity;
  }
};
