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

#include "CameraControlImpl.h"
#include "miro/TimeHelper.h"

namespace Miro
{

  CameraControlImpl::CameraControlImpl(
				       Miro::ZoomParameters zoomParameters_, 
				       Miro::FocusParameters focusParameters_, 
				       Miro::ShutterParameters shutterParameters_) : 
    zoomParameters(zoomParameters_),
    focusParameters(focusParameters_),
    shutterParameters(shutterParameters_)
  {}

  CameraControlImpl::~CameraControlImpl() {}

  void CameraControlImpl::setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!zoomParameters.present) {
      throw Miro::EDevIO("Zoom not supported");
    }
    if ((value>zoomParameters.rangeMax) || (value<zoomParameters.rangeMin)) {
      throw Miro::EOutOfBounds();
    }
  }
  
  float CameraControlImpl::getZoom() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    return zoomParameters.rangeMin;
  }

  ZoomRangeIDL CameraControlImpl::getZoomRange() throw(Miro::EDevIO, Miro::ETimeOut) {
    ZoomRangeIDL result;

    result.min=zoomParameters.rangeMin;
    result.max=zoomParameters.rangeMax;

    return result;
  }

  void CameraControlImpl::setFocus(short value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!focusParameters.present) {
      throw Miro::EDevIO("Focus not supported");
    }
    if ((value>focusParameters.rangeMax) || (value<focusParameters.rangeMin)) {
      throw Miro::EOutOfBounds();
    }
  }
  
  short CameraControlImpl::getFocus() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    return focusParameters.rangeMin;
  }

  FocusRangeIDL CameraControlImpl::getFocusRange() throw(Miro::EDevIO, Miro::ETimeOut) {
    FocusRangeIDL result;

    result.min=focusParameters.rangeMin;
    result.max=focusParameters.rangeMax;

    return result;
  }

  CORBA::Boolean CameraControlImpl::hasAutoFocus() throw()
  {
    return focusParameters.autoFocus;
  }

  void CameraControlImpl::setAutoFocus(unsigned char) throw(Miro::EDevIO, Miro::ETimeOut) {
    if (!focusParameters.autoFocus) {
      throw Miro::EDevIO("Autofocus not supported");
    }
  }

  void CameraControlImpl::setShutter(TimeIDL& value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!shutterParameters.present) {
      throw Miro::EDevIO("Shutter not supported");
    }
    ACE_Time_Value aceValue;
    timeC2A(value,aceValue);
    if ((aceValue>shutterParameters.rangeMax) || 
	(aceValue<shutterParameters.rangeMin)) {
      throw Miro::EOutOfBounds();
    }
  }
  
  TimeIDL CameraControlImpl::getShutter() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    TimeIDL result;
    timeA2C(shutterParameters.rangeMin,result);
    return result;
  }

  ShutterRangeIDL CameraControlImpl::getShutterRange() throw(Miro::EDevIO, Miro::ETimeOut) {
    ShutterRangeIDL result;

    timeA2C(shutterParameters.rangeMin, result.min);
    timeA2C(shutterParameters.rangeMax, result.max);

    return result;
  }


}
