// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PioneerStallImpl_h
#define PioneerStallImpl_h

#include "miro/PioneerStallS.h"

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
    void integrateData(short rStalled,short lStalled, short bat);
    virtual CORBA::Short getStalledWheels() throw();
    virtual CORBA::Boolean isStalled() throw();
    virtual CORBA::Short getBattery() throw();
    virtual void unstall() throw();
  };
};

#endif /* pioneerStallImpl_hh  */
