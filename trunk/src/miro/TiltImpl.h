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
    TiltImpl(Miro::TiltParameters panParameters);
    virtual ~TiltImpl();


    //! Tilt interface method implementation.
    virtual void setTilt(double value) throw(Miro::EDevIO, Miro::EOutOfBounds);
    //! Tilt interface method implementation.
    virtual double getTilt() throw(Miro::EDevIO);
    //! Tilt interface method implementation.
    virtual TiltLimitsIDL getTiltLimits() throw(Miro::EDevIO);


  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    Miro::TiltParameters tiltParameters;
  };
}

#endif // TiltImpl_h
