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
#ifndef ThermometerImpl_h
#define ThermometerImpl_h

#include "idl/ThermometerS.h"
#include "idl/ThermometerEventC.h"
#include "miro/SvcParameters.h"
#include "Synch.h"
#include "Thread.h"
#include <cmath>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declaration
  class StructuredPushSupplier;
  class ThermometerImpl;

  class ThermometerDispatcher : public Thread
  {
    typedef Thread Super;
  public:
    ThermometerDispatcher(StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const ThermometerEventIDL& _data);
    void dispatch();

  protected:
    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Condition cond_;
    //! Preinitialized data structure for Thermometer event.
    CosNotification::StructuredEvent notifyEvent_;

    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class ThermometerImpl;
  };

  //! Implementation of the Thermometer interface.
  /**
   * This class offers a generic implementation for the Thermometer
   * interface. Simply pass the lowlevel thermometer data through the
   * integrateData method and all the rest will be taken care for you.
   *
   * @author Hans Utz, Ingo van Lil
   */
  class  ThermometerImpl : public virtual POA_Miro::Thermometer
  {
  public:
    //! Initializing constructor.
    ThermometerImpl(const ThermometerParameters& _params,
                    StructuredPushSupplier * _supplier,
                    bool _asynchDispatching = false);
    virtual ~ThermometerImpl();

    //! Pass a thermometer reading from the device into ThermometerImpl.
    void integrateData(const ThermometerEventIDL& _data);
    void cancel();

    //! Thermometer interface method implementation.
    virtual ThermometerEventIDL getTemperature() throw();
    //! Thermometer interface method implementation.
    virtual ThermometerEventIDL getWaitTemperature() throw(ETimeOut);
    //! Thermometer interface method implementation.
    virtual void getMinMaxTemperature(CORBA::Float& minTemperature,
                                      CORBA::Float& maxTemperature) throw();

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    //! Reference to thermometer parameters
    const ThermometerParameters& params_;
    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitTemperature
    Condition cond_;
    //! Last thermometer reading
    ThermometerEventIDL data_;

    //! Flag to indicate asynchronous event dispatching.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    ThermometerDispatcher dispatcherThread_;

    //! Timeout for getWaitTemperature
    static ACE_Time_Value maxWait_;
  };
}

#endif // ThermometerImpl_h
