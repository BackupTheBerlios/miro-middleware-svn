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

#include "idl/BatteryS.h"

namespace Miro
{
  //! Implementation of the Battery interface.
  /**
   * This class offers a generic implementation for the Battery
   * interface. Simply pass the lowlevel battery sensor data through
   * the integrateData method and all the rest will be taken care for
   * you.
   *
   * @author Hans Utz
   */
  class BatteryImpl : public virtual POA_Miro::Battery
  {
  public:
    //! Default constructor .
    BatteryImpl();
  
    //! Virtual destructor.
    virtual ~BatteryImpl();
  
    //! Method to pass raw battery data from the device into the BatteryImpl class.
    void integrateData(double _voltage)  ACE_THROW_SPEC(());

    //! Battery interface method implementation.
    virtual CORBA::Double getVoltage() ACE_THROW_SPEC(());

  protected:
    //! The current battery voltage.
    double voltage_;
  };
};

#endif // BatteryImpl_h
