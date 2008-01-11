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
#include "SparrowCMPS03Impl.h"

#include "miro/TimeHelper.h"
#include "miro/StructuredPushSupplier.h"

#include <cmath>

namespace Sparrow
{

  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value maxWait_(0, 100000);

  CompassDispatcher::CompassDispatcher(Miro::StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("CMPS03");
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
  CompassDispatcher::setData(const Miro::CompassEventIDL& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int
  CompassDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous TCM2 dispatching");

    while(!canceled()) {
      Miro::Guard guard(mutex_);
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
  CompassDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void
  CompassDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Miro::Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }

  /** Maximum wait time for cond_.wait calls. */
   ACE_Time_Value CMPS03Impl::maxWait_(1, 0);
 
  /**
   * Implementation skeleton constructor.
   *
   * @param _supplier A pointer to the StructuredPushSupplier to use
   * for event emission. If a NULL pointer is passed the TCM2Impl
   * will not emit any events.
   *
   * @param _asynchDispatching: Do asynchronous event dispatching in
   * a separate thread.
   */
// const CMPS03Parameters& _params,
  CMPS03Impl::CMPS03Impl(  Miro::StructuredPushSupplier * _supplier,
			     bool _asynchDispatching) :
    CompassImpl(_supplier, _asynchDispatching),
//     params_(_params),
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_),
    asynchDispatching_(_asynchDispatching),
    dispatcherThread_(_supplier)
  {
    data_.time.sec = 0;
    data_.time.usec = 0;
    data_.heading = NAN; // uninitialized
   
    if (asynchDispatching_)
      dispatcherThread_.detach(1);
  }

  // Implementation skeleton destructor
  CMPS03Impl::~CMPS03Impl()
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  /**
   * Integrates a new TCM2 reading from the low-level device layer
   * into the TCM2Impl.
   *
   * @param data The new TCM2 reading
   */
//   void
//   CMPS03Impl::integrateData(const Miro::CompassEventIDL& _data)
//   {
//     assert(_data.heading > (float)-M_PI &&
// 	   _data.heading <= (float)M_PI);
//   
//     { // scope for guard
//       Miro::Guard guard(mutex_);
// 
//       // Copy the new data
//       data_ = _data;
// 
//       cond_.broadcast();
//     }
// 
//     // send notification
//     if (supplier_) {
//       if (asynchDispatching_) {
// 	Miro::Guard guard(dispatcherThread_.mutex_);
// 	dispatcherThread_.setData(data_);
// 	dispatcherThread_.cond_.broadcast();
//       }
//       else {
// 	dispatcherThread_.setData(data_);
// 	dispatcherThread_.dispatch();
//       }
//     }
//    }

void
  CMPS03Impl::integrateData(const Miro::CompassEventIDL& _data)
  {
    { // scope for guard
      Miro::Guard guard(mutex_);

      // Copy the new data
      data_.time = _data.time;
      data_.heading = _data.heading;

      cond_.broadcast();
    }

    // send notification
    if (supplier_) {
      if (asynchDispatching_) {
	Miro::Guard guard(dispatcherThread_.mutex_);
	dispatcherThread_.setData(data_);
	dispatcherThread_.cond_.broadcast();
      }
      else {
	dispatcherThread_.setData(data_);
	dispatcherThread_.dispatch();
      }
    }

    Miro::CompassImpl::integrateData(_data);
  }


  void
  CMPS03Impl::cancel() {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  Miro::CompassEventIDL
  CMPS03Impl::getData() throw()
  {
    Miro::Guard guard(mutex_);
    return data_;
  }

  Miro::CompassEventIDL
  CMPS03Impl::getWaitData() throw(Miro::ETimeOut)
  {


    Miro::Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();

    return data_;
  }

  

};
