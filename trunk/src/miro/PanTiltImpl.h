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

#ifndef PanTiltImpl_h
#define PanTiltImpl_h

#include "idl/PanTiltS.h"
#include "miro/Exception.h"
#include "miro/SvcParameters.h"

#include "miro/PanImpl.h"
#include "miro/TiltImpl.h"

#include <cmath>


namespace Miro
{

  //! Implementation of a dummy PanTilt interface.
  /**
   * This class offers a generic implementation for the PanTilt
   *
   * @author Guillem Pagès Gassull
   */
  class  PanTiltImpl : public virtual POA_Miro::PanTilt, public Miro::PanImpl, public Miro::TiltImpl
  {
  public:
    //! Initializing constructor.
    PanTiltImpl(
		Miro::PanParameters panParameters, 
		Miro::TiltParameters tiltParameters);
    virtual ~PanTiltImpl();


    //! PanTilt interface method implementation.
    virtual void setPosition(const PanTiltPositionIDL& value) throw(Miro::EDevIO, Miro::EOutOfBounds);
    //! PanTilt interface method implementation.
    virtual PanTiltPositionIDL getPosition() throw(Miro::EDevIO);
    //! PanTilt interface method implementation.
    virtual PanTiltLimitsIDL getPanTiltLimits() throw(Miro::EDevIO);


  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
  };
}

#endif // PanTiltImpl_h
