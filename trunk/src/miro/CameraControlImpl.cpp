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

namespace Miro
{

  CameraControlImpl::CameraControlImpl(const Miro::CameraControlParameters& _cameraParameters) :
    zoomParameters_(_cameraParameters.zoom),
    focusParameters_(_cameraParameters.focus),
    shutterParameters_(_cameraParameters.shutter)
  {
    currentZoom_=(zoomParameters_.rangeMin+zoomParameters_.rangeMax)/2;
    autoFocusSet_=focusParameters_.autoFocus; 
    //if the camera has autoFocus, default autoFocus on.
    //otherwise, autoFocus off.

    currentFocus_=(focusParameters_.rangeMin+focusParameters_.rangeMax)/2;
    timeA2C(shutterParameters_.rangeMin,currentShutter_); //default fastest setting
  }

  CameraControlImpl::~CameraControlImpl() {}

  void CameraControlImpl::setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!zoomParameters_.present) {
      throw Miro::EDevIO("Zoom not supported");
    }
    if (!testZoom(value)) {
      throw Miro::EOutOfBounds();
    }
    currentZoom_=value;
  }
  
  float CameraControlImpl::getZoom() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    return currentZoom_;
  }

  ZoomRangeIDL CameraControlImpl::getZoomRange() throw(Miro::EDevIO, Miro::ETimeOut) {
    ZoomRangeIDL result;

    result.min=zoomParameters_.rangeMin;
    result.max=zoomParameters_.rangeMax;

    return result;
  }
  
  float CameraControlImpl::getTargetZoom() throw()
  {
    return currentZoom_;
  }

  void CameraControlImpl::setFocus(short value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!focusParameters_.present) {
      throw Miro::EDevIO("Focus not supported");
    }
    if (!testFocus(value)) {
      throw Miro::EOutOfBounds();
    }
    currentFocus_=value;
  }
  
  short CameraControlImpl::getFocus() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    return currentFocus_;
  }

  FocusRangeIDL CameraControlImpl::getFocusRange() throw(Miro::EDevIO, Miro::ETimeOut) {
    FocusRangeIDL result;

    result.min=focusParameters_.rangeMin;
    result.max=focusParameters_.rangeMax;

    return result;
  }

  CORBA::Boolean CameraControlImpl::hasAutoFocus() throw()
  {
    return focusParameters_.autoFocus;
  }

  void CameraControlImpl::setAutoFocus(unsigned char value) throw(Miro::EDevIO, Miro::ETimeOut) {
    if (!focusParameters_.autoFocus) {
      throw Miro::EDevIO("Autofocus not supported");
    }
    autoFocusSet_=value;
  }

  short CameraControlImpl::getTargetFocus() throw()
  {
    return currentFocus_;
  }

  void CameraControlImpl::setShutter(TimeIDL& value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!shutterParameters_.present) {
      throw Miro::EDevIO("Shutter not supported");
    }
    if (!testShutter(value)) {
      throw Miro::EOutOfBounds();
    }
    currentShutter_=value;
  }
  
  TimeIDL CameraControlImpl::getShutter() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    return currentShutter_;
  }

  ShutterRangeIDL CameraControlImpl::getShutterRange() throw(Miro::EDevIO, Miro::ETimeOut) {
    ShutterRangeIDL result;

    timeA2C(shutterParameters_.rangeMin, result.min);
    timeA2C(shutterParameters_.rangeMax, result.max);

    return result;
  }

  TimeIDL CameraControlImpl::getTargetShutter() throw() 
  {
    return currentShutter_;
  }

}
