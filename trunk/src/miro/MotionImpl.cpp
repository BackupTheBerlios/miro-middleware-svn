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

#include "MotionImpl.h"

namespace Miro
{
  /**
   * @param _params Reference to the MotionParameters for the
   * motion device.
   */
  MotionImpl::MotionImpl(const MotionParameters& _params) :
    params_(_params)
  {
    targetVelocity_.translation = 0;
    targetVelocity_.rotation = 0.;
  }

  void
  MotionImpl::getMinMaxVelocity(CORBA::Long& minTranslation,
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
  MotionImpl::setTargetVelocity(const VelocityIDL& _velocity) 
  {
    targetVelocity_ = _velocity;
  }
};
