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

#include "idl/TiltS.h"
#include "miro/Exception.h"
#include "miro/SvcParameters.h"

#include <cmath>


namespace Miro
{

  //! Implementation of a dummy Tilt interface.
  /**
   * This class offers a generic implementation for the Tilt
   *
   * @author Guillem Pagès Gassull
   */
  class  TiltImpl : public virtual POA_Miro::Tilt
  {
  public:
    //! Initializing constructor.
    TiltImpl(const Miro::TiltParameters& _panParameters);
    virtual ~TiltImpl();


    
    void setTargetTilt(double value);
    //! Tilt interface method implementation.
    virtual double getTargetTilt() throw();
    //! Tilt interface method implementation.
    virtual TiltLimitsIDL getTiltLimits() throw(Miro::EDevIO);

    bool testTilt(double value);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    Miro::TiltParameters tiltParameters_;
    double targetTilt_;
  };

  inline bool TiltImpl::testTilt(double value) {
    return ((value>=tiltParameters_.rangeMin) &&
	    (value<=tiltParameters_.rangeMax));
  }
  
  inline void TiltImpl::setTargetTilt(double value)
  { targetTilt_=value; }
}

#endif // TiltImpl_h
