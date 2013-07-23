// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2006
// Institute for Softwaretechnology, Technical University Graz, Austria
//
// $Id: InertialSensorImpl_h,v 1.0 2006/02/28 11:59:41 limberger Exp $
//
//////////////////////////////////////////////////////////////////////////////

#ifndef InertialSensorImpl_h
#define InertialSensorImpl_h

#include "idl/InertialSensorS.h"
#include "idl/InertialSensorEventC.h"
#include "idl/CompassEventC.h"
#include "idl/InclinometerEventC.h"

#include "miro/Synch.h"
#include "miro/Thread.h"
#include "miro/CompassImpl.h"
#include "miro/InclinometerImpl.h"
#include "Parameters.h"
#include <cmath>

#include <orbsvcs/CosNotifyCommC.h>

#include "XsensInertialSensorConnection.h"

#include <string>
#include <iostream>
#include <stdio.h>

namespace Inertial
{
  class InertialSensorImpl;
}

namespace Miro
{
  class StructuredPushSupplier;

  // forward declaration

  class InertialParameters;

  class InertialSensorDispatcher : public Miro::Thread
  {
    typedef Thread Super;
  public:
    InertialSensorDispatcher(Miro::StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const Miro::InertialSensorEventIDL& _data);
    void dispatch();

  protected:
    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock
    Miro::Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Miro::Condition cond_;
    //! Preinitialized data structure for Compass event.
    CosNotification::StructuredEvent notifyEvent_;

    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class Inertial::InertialSensorImpl;
  };

  //! Implementation of the InertialSensor interface.
  /**
   * This class offers a generic implementation for the Compass
   * interface. Simply pass the lowlevel compass data through the
   * integrateData method and all the rest will be taken care for you.
   *
   * @author Gerald Steinbauer, Andi Limberger
   */
}

namespace Inertial
{

  class  InertialSensorImpl : public virtual POA_Miro::InertialSensor,
                              public virtual Miro::CompassImpl,
                              public virtual Miro::InclinometerImpl
  {
  public:
    //! Initializing constructor.
    InertialSensorImpl(const InertialParameters& _parameters,
                 Miro::StructuredPushSupplier * _supplier,
                 bool _asynchDispatching = false);
    virtual ~InertialSensorImpl();

    //! Pass a inertial reading from the device into InertialSensorImpl.
    void integrateData(const Miro::InertialSensorEventIDL& _data);

    void cancel();

    //! Compass interface method implementation.
    virtual Miro::InertialSensorEventIDL getInertialData() throw();
    //! Compass interface method implementation.
    virtual Miro::InertialSensorEventIDL getWaitInertialData() throw (Miro::ETimeOut);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    //! Reference to TCM2 parameters
    const InertialParameters& parameters_;

    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock
    Miro::Mutex mutex_;
    //! Condition for getWaitHeading
    Miro::Condition cond_;
    //! Last inertial reading
    Miro::InertialSensorEventIDL data_;

    //! Flag to indicate asynchronous event dispatching.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    Miro::InertialSensorDispatcher dispatcherThread_;

    //! Timeout for getWaitHeading
    static ACE_Time_Value maxWait_;

  };
}

#endif // InertialSensorImpl_h
