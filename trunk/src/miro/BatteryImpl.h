// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MiroBatteryImpl_h
#define MiroBatteryImpl_h

#include "BatteryS.h"

namespace Miro
{
  class BatteryImpl : public virtual POA_Miro::Battery
  {
  public:
    //Constructor 
    BatteryImpl();
  
    //Destructor 
    virtual ~BatteryImpl();
  
    //------------------------------------------------------------------------
    // IDL interface definition
    //------------------------------------------------------------------------
    void integrateData(double _voltage);
    virtual CORBA::Double getVoltage() throw();

  protected:
    double voltage_;
  };
};

#endif /* pioneerStallImpl_hh  */
