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

#include "PanTiltBase.h"
#include "idl/PanS.h"
#include "miro/SvcParameters.h"

namespace Miro
{

  //! Implementation of a base Pan interface.
  /**
   * This class offers a generic implementation for the Pan
   *
   * @author Guillem Pagès Gassull
   */
  class  PanImpl : public virtual PanTiltBase,
		   public virtual POA_Miro::Pan
  {
  public:
    //! Initializing constructor.
    PanImpl(PanParameters const& _params);
    virtual ~PanImpl();
   

    //! Pan interface method implementation.
    virtual CORBA::Float getTargetPan() throw();
    //! Pan interface method implementation.
    virtual PanLimitsIDL getPanLimits() throw();

  protected:
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------

    //! Test if pan angle is within the limits.
    bool testPan(CORBA::Float _angle) const throw();
    //! Set the new target pan angle.
    void setTargetPan(CORBA::Float _angle) throw();

    //-------------------------------------------------------------------------
    // protected data
    //-------------------------------------------------------------------------
    PanParameters const& params_;
    double targetPan_;
  };

  inline 
  bool 
  PanImpl::testPan(CORBA::Float _angle) const throw() {
    return ((_angle >= float(params_.rangeMin)) &&
	    (_angle <= float(params_.rangeMax)));
  }

  inline 
  void 
  PanImpl::setTargetPan(CORBA::Float _angle) throw() {
    targetPan_ = _angle; 
  }
}

#endif // PanImpl_h
