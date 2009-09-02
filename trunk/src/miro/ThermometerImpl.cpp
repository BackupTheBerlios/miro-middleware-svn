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
#include "ThermometerImpl.h"

#include "TimeHelper.h"
#include "StructuredPushSupplier.h"

#include <cmath>

#if GCC_MAJOR_VERSION == 2
#if GCC_MINOR_VERSION == 95
#include <bits/nan.h>
#endif
#endif

namespace Miro
{

  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value ThermometerDispatcher::maxWait_(0, 100000);

  ThermometerDispatcher::ThermometerDispatcher
  (StructuredPushSupplier * _supplier) :
      supplier_(_supplier),
      mutex_(),
      cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name =
        CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name =
        CORBA::string_dup("Thermometer");
      notifyEvent_.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent_.header.variable_header.length(0);   // put nothing here
      notifyEvent_.filterable_data.length(0);          // put nothing here

      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent_.header.fixed_header.event_type;
      supplier_->addOffers(offers);
    }
  }

  void
  ThermometerDispatcher::setData(const ThermometerEventIDL& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int
  ThermometerDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous Thermometer dispatching");

    while (!canceled()) {
      Guard guard(mutex_);
      ACE_Time_Value timeout(ACE_OS::gettimeofday());
      timeout += maxWait_;
      if (cond_.wait(&timeout) != -1 &&
            !canceled()) {
        dispatch();
      }
    }

    return 0;
  }

  void
  ThermometerDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void
  ThermometerDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }


  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value ThermometerImpl::maxWait_(1, 0);

  /**
   * Implementation skeleton constructor.
   *
   * @param _params Reference to ThermometerParameters
   *
   * @param _supplier A pointer to the StructuredPushSupplier to use
   * for event emission. If a NULL pointer is passed the ThermometerImpl
   * will not emit any events.
   *
   * @param _asynchDispatching: Do asynchronous event dispatching in
   * a separate thread.
   */
  ThermometerImpl::ThermometerImpl(const ThermometerParameters& _params,
                                   StructuredPushSupplier * _supplier,
                                   bool _asynchDispatching) :
      params_(_params),
      supplier_(_supplier),
      mutex_(),
      cond_(mutex_),
      asynchDispatching_(_asynchDispatching),
      dispatcherThread_(_supplier)
  {
    data_.time.sec = 0;
    data_.time.usec = 0;
    data_.temperature = NAN; // uninitialized

    if (asynchDispatching_)
      dispatcherThread_.detach(1);
  }

  // Implementation skeleton destructor
  ThermometerImpl::~ThermometerImpl()
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  /**
   * Integrates a new thermometer reading from the low-level device layer
   * into the ThermometerImpl class.
   *
   * @param data The new thermometer reading
   */
  void
  ThermometerImpl::integrateData(const ThermometerEventIDL& _data)
  {
    { // scope for guard
      Guard guard(mutex_);

      // Copy the new data
      data_ = _data;

      cond_.broadcast();
    }

    // send events
    if (supplier_) {
      if (asynchDispatching_) {
        Guard guard(dispatcherThread_.mutex_);
        dispatcherThread_.setData(data_);
        dispatcherThread_.cond_.broadcast();
      }
      else {
        dispatcherThread_.setData(data_);
        dispatcherThread_.dispatch();
      }
    }
  }

  void
  ThermometerImpl::cancel()
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  ThermometerEventIDL
  ThermometerImpl::getTemperature() throw()
  {
    Guard guard(mutex_);
    return data_;
  }

  ThermometerEventIDL
  ThermometerImpl::getWaitTemperature() throw(ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw ETimeOut();

    return data_;
  }

  void
  ThermometerImpl::getMinMaxTemperature(CORBA::Float& minTemperature,
                                        CORBA::Float& maxTemperature) throw()
  {
    minTemperature = params_.minTemperature;
    maxTemperature = params_.maxTemperature;
  }
}
