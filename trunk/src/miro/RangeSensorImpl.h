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
#ifndef RangeSensorImpl_h
#define RangeSensorImpl_h

#include "RangeSensorS.h"
#include "Thread.h"
#include "Synch.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declaration
  class StructuredPushSupplier;
  class RangeSensorImpl;

  class RangeSensorDispatcher : public Thread
  {
    typedef Thread Super;
  public:
    RangeSensorDispatcher(const ScanDescriptionIDL& _description,
			  StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(RangeScanEventIDL * _data);
    void setData(RangeGroupEventIDL * _data);
    void setData(RangeBunchEventIDL * _data);

    void dispatch();

  protected:
    //! Supplier for events.
    StructuredPushSupplier * supplier_;
    //! True if also RawPosition events shall be emitted.
    bool rawPositionEvents_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitPosition etc.
    Condition cond_;
    //! Preinitialized data structure for RangeSensor event.
    CosNotification::StructuredEvent notifyEvent_;
    
    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class RangeSensorImpl;
  };


  //! Generic implementation of the RangeSensor interface.
  /**
   * This class offers a generic implementation for the RangeSensor
   * interface. Describe the sensor layout and its properties within a
   * @ref ScanDescriptionIDL struct. Then simply pass the lowlevel
   * rangesensor sensor data through the integrateData method that
   * matches the scanning model of the range sensor and all the rest
   * will be taken care for you.
   */
  class RangeSensorImpl : virtual public POA_Miro::RangeSensor
  {
  public:
    //! Initializing constructor.
    RangeSensorImpl(const ScanDescriptionIDL& _description,
		    StructuredPushSupplier * _supplier = NULL,
		    bool _asychDispatching = false);
    ~RangeSensorImpl();

    //! Input from range sensors, that acquire full scans at once.
    virtual void integrateData(RangeScanEventIDL * _data);
    //! Input from range sensors, that acquire scans groupwise.
    virtual void integrateData(RangeGroupEventIDL * _data);
    //! Input from range sensors, that acquire scans in unordered bunchs.
    virtual void integrateData(RangeBunchEventIDL * _data);

    void cancel();

    // IDL interface

    //! RangeSensor interface method implementation.
    ScanDescriptionIDL * getScanDescription() throw();
    //! RangeSensor interface method implementation.
    RangeGroupEventIDL * getGroup(CORBA::ULong id) throw(EOutOfBounds);
    //! RangeSensor interface method implementation.
    RangeGroupEventIDL * getWaitGroup(CORBA::ULong id) throw(EOutOfBounds, ETimeOut);
    //! RangeSensor interface method implementation.
    RangeScanEventIDL * getFullScan() throw();
    //! RangeSensor interface method implementation.
    RangeScanEventIDL * getWaitFullScan() throw(ETimeOut);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    //! Supplier for events.
    StructuredPushSupplier * supplier_;
    //! Description of the layout and properties of the range sensor instance.
    const ScanDescriptionIDL& description_;
    //! True if single sensors have to be masked out.
    bool masking_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitFullScan etc.
    Condition condition_;
    //! Current sensor scan.
    RangeScanEventIDL scan_;

    //! Flag to indicate the we dispatch events asynchrnously.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    RangeSensorDispatcher dispatcherThread_;

    //! Timeout for getWaitPosition etc.
    static ACE_Time_Value maxWait_;
  };
};

#endif // RangeSensorImpl_h
