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

  PanImpl::PanImpl(Miro::PanParameters panParameters_) :
    panParameters(panParameters_)
  {}

  PanImpl::~PanImpl() {}

  void PanImpl::setPan(double value) throw(Miro::EDevIO, Miro::EOutOfBounds)
  {
    if ((value>panParameters.rangeMax) || (value<panParameters.rangeMin)) {
      throw Miro::EOutOfBounds();
    }
  }
  
  double PanImpl::getPan() throw(Miro::EDevIO) 
  {
    return (panParameters.rangeMin);
  }

  PanLimitsIDL PanImpl::getPanLimits() throw(Miro::EDevIO) {
    PanLimitsIDL result;

    result.minpanposition=panParameters.rangeMin;
    result.maxpanposition=panParameters.rangeMax;

    return result;
  }

}
