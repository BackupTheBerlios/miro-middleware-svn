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

#ifndef PanImpl_h
#define PanImpl_h

#include "idl/PanS.h"
#include "miro/Exception.h"
#include "miro/SvcParameters.h"

#include <cmath>


namespace Miro
{

  //! Implementation of a dummy Pan interface.
  /**
   * This class offers a generic implementation for the Pan
   *
   * @author Guillem Pagès Gassull
   */
  class  PanImpl : public virtual POA_Miro::Pan
  {
  public:
    //! Initializing constructor.
    PanImpl(const Miro::PanParameters& _panParameters);
    virtual ~PanImpl();


   
    void setTargetPan(double value);
    //! Pan interface method implementation.
    virtual double getTargetPan() throw();
    //! Pan interface method implementation.
    virtual PanLimitsIDL getPanLimits() throw(Miro::EDevIO);

    bool testPan(double value);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    Miro::PanParameters panParameters_;
    double targetPan_;
  };

  inline bool PanImpl::testPan(double value) {
    return ((value>=panParameters_.rangeMin) &&
	    (value<=panParameters_.rangeMax));
  }
  inline void PanImpl::setTargetPan(double value) 
  { targetPan_=value; }
}

#endif // PanImpl_h
