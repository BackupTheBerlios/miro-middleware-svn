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

  TiltImpl::TiltImpl(Miro::TiltParameters tiltParameters_) :
    tiltParameters(tiltParameters_)
  {}

  TiltImpl::~TiltImpl() {}

  void TiltImpl::setTilt(double value) throw(Miro::EDevIO, Miro::EOutOfBounds)
  {
    if ((value>tiltParameters.rangeMax) || (value<tiltParameters.rangeMin)) {
      throw Miro::EOutOfBounds();
    }
  }
  
  double TiltImpl::getTilt() throw(Miro::EDevIO) 
  {
    return (tiltParameters.rangeMin);
  }

  TiltLimitsIDL TiltImpl::getTiltLimits() throw(Miro::EDevIO) {
    TiltLimitsIDL result;

    result.mintiltposition=tiltParameters.rangeMin;
    result.maxtiltposition=tiltParameters.rangeMax;

    return result;
  }

}
