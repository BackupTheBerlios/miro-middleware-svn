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
    PanTiltImpl(const Miro::PanTiltParameters& _panTiltParameters);
    virtual ~PanTiltImpl();


    virtual void setTargetPosition(const PanTiltPositionIDL& value);
    //! PanTilt interface method implementation.
    virtual PanTiltPositionIDL getTargetPosition() throw();
    //! PanTilt interface method implementation.
    virtual PanTiltLimitsIDL getPanTiltLimits() throw(Miro::EDevIO);

    bool testPosition(const PanTiltPositionIDL& value);

  protected:
    //------------------------------------------------------------------------
    // protected object data
    //------------------------------------------------------------------------
    PanTiltPositionIDL targetPosition_;
  };

  inline bool PanTiltImpl::testPosition(const PanTiltPositionIDL& value) {
    return ((value.panvalue<=panParameters_.rangeMax) && 
	(value.panvalue>=panParameters_.rangeMin) && 
	(value.tiltvalue<=tiltParameters_.rangeMax) && 
	    (value.tiltvalue>=tiltParameters_.rangeMin));
  }
  
  inline void PanTiltImpl::setTargetPosition(const PanTiltPositionIDL& value)
  { targetPosition_=value; }

}

#endif // PanTiltImpl_h
