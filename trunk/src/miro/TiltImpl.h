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
#ifndef TiltImpl_h
#define TiltImpl_h

#include "PanTiltBase.h"
#include "idl/TiltS.h"
#include "miro/SvcParameters.h"

namespace Miro
{

  //! Partial implementation of the Tilt interface.
  /**
   * This class offers a generic part of a tilt implementation.
   *
   * @author Guillem Pagès Gassull
   */
  class  TiltImpl :public virtual PanTiltBase,
		   public virtual POA_Miro::Tilt
  {
  public:
    //! Initializing constructor.
    TiltImpl(Miro::TiltParameters const& _params);
    virtual ~TiltImpl();

    //! Tilt interface method implementation.
    virtual float getTargetTilt() throw();
    //! Tilt interface method implementation.
    virtual TiltLimitsIDL getTiltLimits() throw();

  protected:
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------

    //! Test if tilt angle is within the limits.
    bool testTilt(CORBA::Float _angle) const throw();
    //! Set the new target pan angle.
    void setTargetTilt(CORBA::Float _angle) throw();

    //-------------------------------------------------------------------------
    // protected data
    //-------------------------------------------------------------------------
    TiltParameters const& params_;
    CORBA::Float targetTilt_;
  };

  inline 
  bool
  TiltImpl::testTilt(CORBA::Float _angle) const throw() {
    return ((_angle >= float(params_.rangeMin)) &&
	    (_angle <= float(params_.rangeMax)));
  }
  
  inline
  void
  TiltImpl::setTargetTilt(CORBA::Float _angle) throw() {
    targetTilt_ = _angle; 
  }
}

#endif // TiltImpl_h
