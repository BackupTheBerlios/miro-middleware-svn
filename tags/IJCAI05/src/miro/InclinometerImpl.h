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

#ifndef InclinometerImpl_h
#define InclinometerImpl_h

#include "idl/InclinometerS.h"
#include "idl/InclinometerEventC.h"
#include "miro/SvcParameters.h"
#include "Synch.h"
#include "Thread.h"
#include <cmath>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declaration
  class StructuredPushSupplier;
  class InclinometerImpl;

  class InclinometerDispatcher : public Thread
  {
    typedef Thread Super;
  public:
    InclinometerDispatcher(StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const InclinometerEventIDL& _data);
    void dispatch();

  protected:
    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Condition cond_;
    //! Preinitialized data structure for Inclinometer event.
    CosNotification::StructuredEvent notifyEvent_;

    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class InclinometerImpl;
  };

  //! Implementation of the Inclinometer interface.
  /**
   * This class offers a generic implementation for the Inclinometer
   * interface. Simply pass the lowlevel inclinometer data through the
   * integrateData method and all the rest will be taken care for you.
   *
   * @author Hans Utz, Ingo van Lil
   */
  class  InclinometerImpl : public virtual POA_Miro::Inclinometer
  {
  public:
    //! Initializing constructor.
    InclinometerImpl(const InclinometerParameters& _params,
		 StructuredPushSupplier * _supplier,
		 bool _asynchDispatching = false);
    virtual ~InclinometerImpl();

    //! Pass a inclinometer reading from the device into InclinometerImpl.
    void integrateData(const InclinometerEventIDL& _data);
    void cancel();

    //! Inclinometer interface method implementation.
    virtual InclinometerEventIDL getInclination() throw();
    //! Inclinometer interface method implementation.
    virtual InclinometerEventIDL getWaitInclination() throw (ETimeOut);
    //! Inclinometer interface method implementation.
    virtual void getMinMaxPitch(CORBA::Float& minPitch,
				CORBA::Float& maxPitch) throw();
    //! Inclinometer interface method implementation.
    virtual void getMinMaxRoll(CORBA::Float& minRoll,
			       CORBA::Float& maxRoll) throw();

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    //! Reference to inclinometer parameters
    const InclinometerParameters& params_;
    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitInclination
    Condition cond_;
    //! Last inclinometer reading
    InclinometerEventIDL data_;

    //! Flag to indicate asynchronous event dispatching.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    InclinometerDispatcher dispatcherThread_;

    //! Timeout for getWaitInclination
    static ACE_Time_Value maxWait_;
  };
}

#endif // InclinometerImpl_h
