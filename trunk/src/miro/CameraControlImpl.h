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
#include <cmath>


namespace Miro
{

  //! Implementation of a dummy CameraControl interface.
  /**
   * This class offers a generic implementation for the CameraControl
   *
   * @author Guillem Pagès Gassull
   */
  class  CameraControlImpl : public virtual POA_Miro::CameraControl
  {
  public:
    //! Initializing constructor.
    CameraControlImpl(
		      Miro::ZoomParameters zoomParameters, 
		      Miro::FocusParameters focusParameters, 
		      Miro::ShutterParameters shutterParameters);
    virtual ~CameraControlImpl();


    //! CameraControl interface method implementation.
    virtual void setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual float getZoom() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual ZoomRangeIDL getZoomRange() throw(Miro::EDevIO, Miro::ETimeOut);

    //! CameraControl interface method implementation.
    virtual void setFocus(short value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual short getFocus() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual FocusRangeIDL getFocusRange() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual CORBA::Boolean hasAutoFocus() throw();
    //! CameraControl interface method implementation.
    virtual void setAutoFocus(unsigned char value) throw(Miro::EDevIO, Miro::ETimeOut);

    //! CameraControl interface method implementation.
    virtual void setShutter(TimeIDL & value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual TimeIDL getShutter() throw(Miro::EDevIO, Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual ShutterRangeIDL getShutterRange() throw(Miro::EDevIO, Miro::ETimeOut);


  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    Miro::ZoomParameters zoomParameters;
    Miro::FocusParameters focusParameters;
    Miro::ShutterParameters shutterParameters;

  };
}

#endif // CameraControlImpl_h
