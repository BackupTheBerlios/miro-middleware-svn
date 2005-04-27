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

#include "CompassImpl.h"

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
  ACE_Time_Value CompassDispatcher::maxWait_(0, 100000);

  CompassDispatcher::CompassDispatcher(StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("Compass");
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
  CompassDispatcher::setData(const CompassEventIDL& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int
  CompassDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous Compass dispatching");

    while(!canceled()) {
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
  CompassDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void
  CompassDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }


  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value CompassImpl::maxWait_(1, 0);

  /**
   * Implementation skeleton constructor.
   *
   * @param _supplier A pointer to the StructuredPushSupplier to use
   * for event emission. If a NULL pointer is passed the CompassImpl
   * will not emit any events.
   *
   * @param _asynchDispatching: Do asynchronous event dispatching in
   * a separate thread.
   */
  CompassImpl::CompassImpl(StructuredPushSupplier * _supplier,
			     bool _asynchDispatching) :
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
  CompassImpl::~CompassImpl()
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  /**
   * Integrates a new compass reading from the low-level device layer
   * into the CompassImpl.
   *
   * @param data The new compass reading
   */
  void 
  CompassImpl::integrateData(const CompassEventIDL& _data)
  {
    assert(_data.heading > -M_PI &&
	   _data.heading <= M_PI);

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
  CompassImpl::cancel() {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  CompassEventIDL
  CompassImpl::getHeading() throw()
  {
    Guard guard(mutex_);
    return data_;
  }

  CompassEventIDL
  CompassImpl::getWaitHeading() throw(ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw ETimeOut();

    return data_;
  }
}
