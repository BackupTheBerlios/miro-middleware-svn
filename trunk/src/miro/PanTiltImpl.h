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
  class  PanTiltImpl : public virtual POA_Miro::PanTilt, 
		       public Miro::PanImpl,
		       public Miro::TiltImpl
  {
  public:
    //! Initializing constructor.
    PanTiltImpl(PanTiltParameters const& _params);
    virtual ~PanTiltImpl();

    //! PanTilt interface method implementation.
    virtual PanTiltPositionIDL getTargetPosition() throw();
    //! PanTilt interface method implementation.
    virtual PanTiltLimitsIDL getPanTiltLimits() throw();

  protected:
    //------------------------------------------------------------------------
    // protected methods
    //------------------------------------------------------------------------
    bool testPosition(PanTiltPositionIDL const& _value) const throw();
    void setTargetPosition(PanTiltPositionIDL const& _value) throw();

    //------------------------------------------------------------------------
    // protected object data
    //------------------------------------------------------------------------
  };

  inline
  bool
  PanTiltImpl::testPosition(PanTiltPositionIDL const& _value) const throw() {
    return (testPan(_value.panValue) && testTilt(_value.tiltValue));
  }
  
  inline 
  void
  PanTiltImpl::setTargetPosition(PanTiltPositionIDL const& _value) throw() { 
    setTargetPan(_value.panValue);
    setTargetTilt(_value.tiltValue);
  }
}

#endif // PanTiltImpl_h
