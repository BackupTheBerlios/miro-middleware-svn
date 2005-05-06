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

#ifndef MagnetometerImpl_h
#define MagnetometerImpl_h

#include "idl/MagnetometerS.h"
#include "idl/MagnetometerEventC.h"
#include "miro/SvcParameters.h"
#include "Synch.h"
#include "Thread.h"
#include <cmath>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declaration
  class StructuredPushSupplier;
  class MagnetometerImpl;

  class MagnetometerDispatcher : public Thread
  {
    typedef Thread Super;
  public:
    MagnetometerDispatcher(StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const MagnetometerEventIDL& _data);
    void dispatch();

  protected:
    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Condition cond_;
    //! Preinitialized data structure for Magnetometer event.
    CosNotification::StructuredEvent notifyEvent_;

    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class MagnetometerImpl;
  };

  //! Implementation of the Magnetometer interface.
  /**
   * This class offers a generic implementation for the Magnetometer
   * interface. Simply pass the lowlevel magnetometer data through the
   * integrateData method and all the rest will be taken care for you.
   *
   * @author Hans Utz, Ingo van Lil
   */
  class  MagnetometerImpl : public virtual POA_Miro::Magnetometer
  {
  public:
    //! Initializing constructor.
    MagnetometerImpl(const MagnetometerParameters& _params,
		 StructuredPushSupplier * _supplier,
		 bool _asynchDispatching = false);
    virtual ~MagnetometerImpl();

    //! Pass a magnetometer reading from the device into MagnetometerImpl.
    void integrateData(const MagnetometerEventIDL& _data);
    void cancel();

    //! Magnetometer interface method implementation.
    virtual MagnetometerEventIDL getFieldStrength() throw();
    //! Magnetometer interface method implementation.
    virtual MagnetometerEventIDL getWaitFieldStrength() throw (ETimeOut);
    //! Magnetometer interface method implementation.
    virtual void getMinMaxFieldStrength(CORBA::Float& minFieldStrength,
				CORBA::Float& maxFieldStrength) throw();

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    //! Reference to the magnetometer parameters
    const MagnetometerParameters& params_;
    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitFieldStrength
    Condition cond_;
    //! Last magnetometer reading
    MagnetometerEventIDL data_;

    //! Flag to indicate asynchronous event dispatching.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    MagnetometerDispatcher dispatcherThread_;

    //! Timeout for getWaitFieldStrength
    static ACE_Time_Value maxWait_;
  };
}

#endif // MagnetometerImpl_h
