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

#include "MagnetometerImpl.h"

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
  ACE_Time_Value MagnetometerDispatcher::maxWait_(0, 100000);

  MagnetometerDispatcher::MagnetometerDispatcher(StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("Magnetometer");
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
  MagnetometerDispatcher::setData(const MagnetometerEventIDL& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int
  MagnetometerDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous Magnetometer dispatching");

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
  MagnetometerDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void
  MagnetometerDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }


  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value MagnetometerImpl::maxWait_(1, 0);

  /**
   * Implementation skeleton constructor.
   *
   * @param _params Reference to MagnetometerParameters
   *
   * @param _supplier A pointer to the StructuredPushSupplier to use
   * for event emission. If a NULL pointer is passed the MagnetometerImpl
   * will not emit any events.
   *
   * @param _asynchDispatching: Do asynchronous event dispatching in
   * a separate thread.
   */
  MagnetometerImpl::MagnetometerImpl(const MagnetometerParameters& _params,
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
    data_.fieldstrength.x = NAN; // uninitialized
    data_.fieldstrength.y = NAN; // uninitialized
    data_.fieldstrength.z = NAN; // uninitialized

    if (asynchDispatching_)
      dispatcherThread_.detach(1);
  }

  // Implementation skeleton destructor
  MagnetometerImpl::~MagnetometerImpl()
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  /**
   * Integrates a new magnetometer reading from the low-level device layer
   * into the MagnetometerImpl.
   *
   * @param data The new magnetometer reading
   */
  void 
  MagnetometerImpl::integrateData(const MagnetometerEventIDL& _data)
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
  MagnetometerImpl::cancel() {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  MagnetometerEventIDL
  MagnetometerImpl::getFieldStrength() throw()
  {
    Guard guard(mutex_);
    return data_;
  }

  MagnetometerEventIDL
  MagnetometerImpl::getWaitFieldStrength() throw(ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw ETimeOut();

    return data_;
  }

  void
  MagnetometerImpl::getMinMaxFieldStrength(CORBA::Float& minFieldStrength,
				    CORBA::Float& maxFieldStrength) throw()
  {
    minFieldStrength = params_.minFieldStrength;
    maxFieldStrength = params_.maxFieldStrength;
  }
}
