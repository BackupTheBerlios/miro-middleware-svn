// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef TCM2Impl_h
#define TCM2Impl_h

#include "idl/TCM2S.h"
#include "miro/CompassImpl.h"
#include "miro/InclinometerImpl.h"
#include "miro/MagnetometerImpl.h"
#include "miro/ThermometerImpl.h"
#include "pioneer/Parameters.h"

#include "idl/TCM2EventC.h"
#include "idl/CompassEventC.h"
#include "idl/InclinometerEventC.h"
#include "idl/MagnetometerEventC.h"
#include "idl/ThermometerEventC.h"

#include "miro/Synch.h"
#include "miro/Thread.h"
#include <cmath>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;
}

namespace Pioneer
{
  // forward declaration
  class TCM2Impl;

  class TCM2Dispatcher : public Miro::Thread
  {
    typedef Miro::Thread Super;
  public:
    TCM2Dispatcher(Miro::StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const Miro::TCM2EventIDL& _data);
    void dispatch();

  protected:
    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock
    Miro::Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Miro::Condition cond_;
    //! Preinitialized data structure for TCM2 event.
    CosNotification::StructuredEvent notifyEvent_;

    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class TCM2Impl;
  };

  //! Implementation of the TCM2 interface.
  /**
   * This class offers a generic implementation for the TCM2 module
   * interface. Simply pass the lowlevel sensor data through the
   * integrateData method and all the rest will be taken care for you.
   * Alternatively the four base classes' integrateData methods (with
   * their respective data structures) can be used separately.
   *
   * @author Hans Utz, Ingo van Lil
   */
  class TCM2Impl : public virtual POA_Miro::TCM2,
		   public virtual Miro::CompassImpl,
		   public virtual Miro::InclinometerImpl,
		   public virtual Miro::MagnetometerImpl,
		   public virtual Miro::ThermometerImpl
  {
  public:
    //! Initializing constructor.
    TCM2Impl(const TCM2Parameters& _params,
		 Miro::StructuredPushSupplier * _supplier,
		 bool _asynchDispatching = false);
    virtual ~TCM2Impl();

    //! Pass a TCM2 reading from the device into TCM2Impl.
    void integrateData(const Miro::TCM2EventIDL& _data);
    void cancel();

    //! TCM2 interface method implementation.
    virtual Miro::TCM2EventIDL getData() throw();
    //! TCM2 interface method implementation.
    virtual Miro::TCM2EventIDL getWaitData() throw (Miro::ETimeOut);

    //! Inherited from Compass interface
    void integrateData(const Miro::CompassEventIDL& _data);
    //! Inherited from Inclinometer interface
    void integrateData(const Miro::InclinometerEventIDL& _data);
    //! Inherited from Magnetometer interface
    void integrateData(const Miro::MagnetometerEventIDL& _data);
    //! Inherited from Thermometer interface
    void integrateData(const Miro::ThermometerEventIDL& _data);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    //! Reference to TCM2 parameters
    const TCM2Parameters& params_;
    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock
    Miro::Mutex mutex_;
    //! Condition for getWaitData
    Miro::Condition cond_;
    //! Last TCM2 reading
    Miro::TCM2EventIDL data_;

    //! Flag to indicate asynchronous event dispatching.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    TCM2Dispatcher dispatcherThread_;

    //! Timeout for getWaitData
    static ACE_Time_Value maxWait_;
  };
}

#endif // TCM2Impl_h
