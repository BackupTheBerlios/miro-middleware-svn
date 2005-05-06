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

#ifndef CameraControlImpl_h
#define CameraControlImpl_h

#include "idl/CameraControlS.h"
#include "miro/Exception.h"
#include "miro/SvcParameters.h"
#include "miro/TimeHelper.h"
#include <cmath>


namespace Miro
{

  //! Implementation of a dummy CameraControl interface.
  /**
   * This class offers a generic implementation for the CameraControl
   * 
   * It provides default implementations for the IDL methods as well as 
   *  some helper methods.
   * 
   * The corresponding methods should be overridden to achieve the desired
   *  functionality.
   * Do not forget to set the target on the overriden method! 
   *  (setXXXTarget(value))
   * 
   * In case a camera does not provide a specific function (e.g. focus),
   *  the function need not be overriden; by default the get/set methods
   *  test against the limits provided in the configuration XML file and
   *  set/get the target values.
   *
   * @author Guillem Pagès Gassull
   */
  class  CameraControlImpl : public virtual POA_Miro::CameraControl
  {
  public:
    //! Initializing constructor.
    CameraControlImpl(const Miro::CameraControlParameters& _cameraParameters);
    virtual ~CameraControlImpl();


    //! CameraControl interface method implementation.
    virtual void setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual float getZoom() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    /**
     * @return The range specified on the XML configuration file
     */
    virtual ZoomRangeIDL getZoomRange() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    /**
     * @return The target zoom as set by setTargetZoom
     */
    virtual float getTargetZoom() throw();
    void setTargetZoom(float value);
    /**
     * Test whether the value lies within the allowed range
     */
    bool testZoom(float value);

    //! CameraControl interface method implementation.
    virtual void setFocus(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual float getFocus() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    /**
     * @return The range specified on the XML configuration file
     */
    virtual FocusRangeIDL getFocusRange() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    /**
     * @return The value specified on the XML configuration file
     */
    virtual CORBA::Boolean hasAutoFocus() throw();
    //! CameraControl interface method implementation.
    virtual void setAutoFocus(CORBA::Boolean value) throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    /**
     * @return The target focus as set by setTargetFocus
     */
    virtual float getTargetFocus() throw();
    void setTargetFocus(float value);
    /**
     * Test whether the value lies within the allowed range
     */
    bool testFocus(float value);

    //! CameraControl interface method implementation.
    virtual void setShutter(TimeIDL & value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual TimeIDL getShutter() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    /**
     * @return The range specified on the XML configuration file
     */
    virtual ShutterRangeIDL getShutterRange() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    /**
     * @return The target shutter as set by setTargetShutter
     */
    virtual TimeIDL getTargetShutter() throw();
    void setTargetShutter(const TimeIDL& value);
    /**
     * Test whether the value lies within the allowed range
     */
    bool testShutter(const TimeIDL& value);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    Miro::ZoomParameters zoomParameters_;
    Miro::FocusParameters focusParameters_;
    Miro::ShutterParameters shutterParameters_;

    float currentZoom_;
    float currentFocus_;
    bool autoFocusSet_;
    TimeIDL currentShutter_;

  };

  inline
  bool CameraControlImpl::testZoom(float value) 
  {
    if ((value>float(zoomParameters_.rangeMax)) || (value<float(zoomParameters_.rangeMin))) {
      return false;
    } else {
      return true;
    }
  }
  
  inline
  bool CameraControlImpl::testFocus(float value) 
  {
    if ((value>float(focusParameters_.rangeMax)) || (value<float(focusParameters_.rangeMin))) {
      return false;
    } else {
      return true;
    }
  }
  
  inline
  bool CameraControlImpl::testShutter(const TimeIDL& value) 
  {
    ACE_Time_Value aceValue;
    Miro::timeC2A(value,aceValue);
    if ((aceValue>shutterParameters_.rangeMax) || 
	(aceValue<shutterParameters_.rangeMin)) {
      return false;
    } else {
      return true;
    }
  }
  inline void CameraControlImpl::setTargetZoom(float value) {
    currentZoom_=value;
  }
  inline void CameraControlImpl::setTargetFocus(float value) {
    currentFocus_=value;
  }
  inline void CameraControlImpl::setTargetShutter(const TimeIDL& value) {
    currentShutter_=value;
  }
  
}

#endif // CameraControlImpl_h
