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

  TiltImpl::TiltImpl(TiltParameters const& _params) :
    params_(_params),
    targetTilt_(0)
  {}

  TiltImpl::~TiltImpl() 
  {}

  CORBA::Float TiltImpl::getTargetTilt() throw() 
  {
    return targetTilt_;
  }

  TiltLimitsIDL TiltImpl::getTiltLimits() throw()
  {
    TiltLimitsIDL result;
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);

    result.minAngle = params_.rangeMin;
    result.maxAngle = params_.rangeMax;

    return result;
  }
}
