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

#ifndef CompassImpl_h
#define CompassImpl_h

#include "idl/CompassS.h"
#include "idl/CompassEventC.h"

#include "Synch.h"
#include "Thread.h"
#include <cmath>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declaration
  class StructuredPushSupplier;
  class CompassImpl;

  class CompassDispatcher : public Thread
  {
    typedef Thread Super;
  public:
    CompassDispatcher(StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const CompassEventIDL& _data);
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

    friend class CompassImpl;
  };

  //! Implementation of the Compass interface.
  /**
   * This class offers a generic implementation for the Compass
   * interface. Simply pass the lowlevel compass data through the
   * integrateData method and all the rest will be taken care for you.
   *
   * @author Hans Utz, Ingo van Lil
   */
  class  CompassImpl : public virtual POA_Miro::Compass
  {
  public:
    //! Initializing constructor.
    CompassImpl(StructuredPushSupplier * _supplier,
		 bool _asynchDispatching = false);
    virtual ~CompassImpl();

    //! Pass a compass reading from the device into CompassImpl.
    void integrateData(const CompassEventIDL& _data);
    void cancel();

    //! Compass interface method implementation.
    virtual CompassEventIDL getHeading() throw();
    //! Compass interface method implementation.
    virtual CompassEventIDL getWaitHeading() throw (ETimeOut);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitHeading
    Condition cond_;
    //! Last compass reading
    CompassEventIDL data_;

    //! Flag to indicate asynchronous event dispatching.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    CompassDispatcher dispatcherThread_;

    //! Timeout for getWaitHeading
    static ACE_Time_Value maxWait_;
  };
}

#endif // CompassImpl_h
