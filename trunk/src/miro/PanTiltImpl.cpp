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

  PanTiltImpl::PanTiltImpl(Miro::PanParameters panParameters_,Miro::TiltParameters tiltParameters_) :
    PanImpl(panParameters_),
    TiltImpl(tiltParameters_)
  {
  }

  PanTiltImpl::~PanTiltImpl() {}

  void PanTiltImpl::setPosition(const PanTiltPositionIDL& value) throw(Miro::EDevIO, Miro::EOutOfBounds)
  {
    if ((value.panvalue>panParameters.rangeMax) || 
	(value.panvalue<panParameters.rangeMin) || 
	(value.tiltvalue>tiltParameters.rangeMax) || 
        (value.tiltvalue<tiltParameters.rangeMin)) {
      throw Miro::EOutOfBounds();
    }
  }
  
  PanTiltPositionIDL PanTiltImpl::getPosition() throw(Miro::EDevIO) 
  {
    PanTiltPositionIDL result;
    result.panvalue=panParameters.rangeMin;
    result.tiltvalue=tiltParameters.rangeMin;
    return result;
  }

  PanTiltLimitsIDL PanTiltImpl::getPanTiltLimits() throw(Miro::EDevIO) {
    PanTiltLimitsIDL result;

    result.minpanposition=panParameters.rangeMin;
    result.maxpanposition=panParameters.rangeMax;
    result.mintiltposition=tiltParameters.rangeMin;
    result.maxtiltposition=tiltParameters.rangeMax;

    return result;
  }

}
