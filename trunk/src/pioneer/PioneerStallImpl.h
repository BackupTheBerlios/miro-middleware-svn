// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PioneerStallImpl_h
#define PioneerStallImpl_h

#include "idl/PioneerStallS.h"

namespace Pioneer
{
  class StallImpl : public virtual POA_Miro::PioneerStall
  {
  public:
    //Constructor 
    StallImpl();
  
    //Destructor 
    virtual ~StallImpl();
  
    //------------------------------------------------------------------------
    // IDL interface definition
    //------------------------------------------------------------------------
    void integrateData(short rStalled,short lStalled);
    virtual CORBA::Short getStalledWheels() throw();
    virtual CORBA::Boolean isStalled() throw();
    virtual void unstall() throw();
  };
};

#endif /* pioneerStallImpl_hh  */
