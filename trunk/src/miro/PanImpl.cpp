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
#include "Angle.h"

namespace Miro
{

  PanImpl::PanImpl(const Miro::PanParameters& _panParameters) :
    panParameters_(_panParameters),
    targetPan_(0)
  {}

  PanImpl::~PanImpl() {}

  double PanImpl::getTargetPan() throw() 
  {
    return targetPan_;
  }

  PanLimitsIDL PanImpl::getPanLimits() throw(Miro::EDevIO) {
    PanLimitsIDL result;

    result.minpanposition=panParameters_.rangeMin;
    result.maxpanposition=panParameters_.rangeMax;

    return result;
  }

}
