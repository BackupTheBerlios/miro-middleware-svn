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
    PanImpl(Miro::PanParameters panParameters);
    virtual ~PanImpl();


    //! Pan interface method implementation.
    virtual void setPan(double value) throw(Miro::EDevIO, Miro::EOutOfBounds);
    //! Pan interface method implementation.
    virtual double getPan() throw(Miro::EDevIO);
    //! Pan interface method implementation.
    virtual PanLimitsIDL getPanLimits() throw(Miro::EDevIO);


  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    Miro::PanParameters panParameters;
  };
}

#endif // PanImpl_h
