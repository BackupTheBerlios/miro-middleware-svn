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

  PanTiltImpl::PanTiltImpl(const Miro::PanTiltParameters& _panTiltParameters):
    PanImpl(_panTiltParameters.pan),
    TiltImpl(_panTiltParameters.tilt)
  {
  }

  PanTiltImpl::~PanTiltImpl() {}

  PanTiltPositionIDL PanTiltImpl::getTargetPosition() throw() 
  {
    PanTiltPositionIDL targetPosition;
    targetPosition.panvalue=targetPan_;
    targetPosition.tiltvalue=targetTilt_;
    return targetPosition;
  }

  PanTiltLimitsIDL PanTiltImpl::getPanTiltLimits() throw(Miro::EDevIO) {
    PanTiltLimitsIDL result;

    result.minpanposition=panParameters_.rangeMin;
    result.maxpanposition=panParameters_.rangeMax;
    result.mintiltposition=tiltParameters_.rangeMin;
    result.maxtiltposition=tiltParameters_.rangeMax;

    return result;
  }

}
