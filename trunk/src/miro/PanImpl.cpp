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

#include "PanImpl.h"

namespace Miro
{

  PanImpl::PanImpl(PanParameters const& _params) :
    params_(_params),
    targetPan_(0.f)
  {}

  PanImpl::~PanImpl() 
  {}

  CORBA::Float 
  PanImpl::getTargetPan() throw() 
  {
    return targetPan_;
  }

  PanLimitsIDL 
  PanImpl::getPanLimits() throw()
  {
    PanLimitsIDL result;
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);

    result.minAngle = params_.rangeMin;
    result.maxAngle = params_.rangeMax;

    return result;
  }
}
