// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PanTiltImpl.h"
#include "Angle.h"

namespace Miro
{

  PanTiltImpl::PanTiltImpl(PanTiltParameters const& _params):
    PanImpl(_params.pan),
    TiltImpl(_params.tilt)
  {
  }

  PanTiltImpl::~PanTiltImpl() 
  {}

  PanTiltPositionIDL PanTiltImpl::getTargetPosition() throw() 
  {
    PanTiltPositionIDL targetPosition;
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);

    targetPosition.panValue = targetPan_;
    targetPosition.tiltValue = targetTilt_;

    return targetPosition;
  }

  PanTiltLimitsIDL PanTiltImpl::getPanTiltLimits() throw()
  {
    PanTiltLimitsIDL result;
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);

    result.pan = getPanLimits();
    result.tilt = getTiltLimits();

    return result;
  }
}
