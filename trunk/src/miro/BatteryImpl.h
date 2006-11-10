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
#include "idl/BatteryEventC.h"

#include "Synch.h"
#include "Thread.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declaration
  class StructuredPushSupplier;


  class BatteryDispatcher : public Thread
  {
    typedef Thread Super;
  public:
    BatteryDispatcher(StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const BatteryEventIDL& _data);
    void dispatch();

  protected:
    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Condition cond_;
    //! Preinitialized data structure for Compass event.
    CosNotification::StructuredEvent notifyEvent_;

    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class BatteryImpl;
  };

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
    BatteryImpl(StructuredPushSupplier * _supplier = 0, bool _asynchDispatching = false);
  
    //! Virtual destructor.
    virtual ~BatteryImpl();
  
    //! Method to pass raw battery data from the device into the BatteryImpl class.
    void integrateData(double _voltage)  ACE_THROW_SPEC(());

    //! Battery interface method implementation.
    virtual CORBA::Double getVoltage() ACE_THROW_SPEC(());
	
    void cancel();

  protected:
    //! The current battery voltage.
    double voltage_;

    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitHeading
    Condition cond_;

    //! Last compass reading
    BatteryEventIDL data_;

    //! Flag to indicate asynchronous event dispatching.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    BatteryDispatcher dispatcherThread_;

  };
};

#endif // BatteryImpl_h
