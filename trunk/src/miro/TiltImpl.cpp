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

#include "TiltImpl.h"
#include "Angle.h"

namespace Miro
{

  TiltImpl::TiltImpl(const Miro::TiltParameters& _tiltParameters) :
    tiltParameters_(_tiltParameters)
  {}

  TiltImpl::~TiltImpl() {}

  double TiltImpl::getTargetTilt() throw() 
  {
    return targetTilt_;
  }

  TiltLimitsIDL TiltImpl::getTiltLimits() throw(Miro::EDevIO) {
    TiltLimitsIDL result;

    result.mintiltposition=tiltParameters_.rangeMin;
    result.maxtiltposition=tiltParameters_.rangeMax;

    return result;
  }

}
