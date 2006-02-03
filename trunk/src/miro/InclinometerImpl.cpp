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

#include "InclinometerImpl.h"

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
  ACE_Time_Value InclinometerDispatcher::maxWait_(0, 100000);

  InclinometerDispatcher::InclinometerDispatcher(StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("Inclinometer");
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
  InclinometerDispatcher::setData(const InclinometerEventIDL& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int
  InclinometerDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous Inclinometer dispatching");

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
  InclinometerDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void
  InclinometerDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }


  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value InclinometerImpl::maxWait_(1, 0);

  /**
   * Implementation skeleton constructor.
   *
   * @param _params Reference to InclinometerParameters
   *
   * @param _supplier A pointer to the StructuredPushSupplier to use
   * for event emission. If a NULL pointer is passed the InclinometerImpl
   * will not emit any events.
   *
   * @param _asynchDispatching: Do asynchronous event dispatching in
   * a separate thread.
   */
  InclinometerImpl::InclinometerImpl(const InclinometerParameters& _params,
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
    data_.inclination.pitch = NAN; // uninitialized
    data_.inclination.roll = NAN; // uninitialized

    if (asynchDispatching_)
      dispatcherThread_.detach(1);
  }

  // Implementation skeleton destructor
  InclinometerImpl::~InclinometerImpl()
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  /**
   * Integrates a new inclinometer reading from the low-level device layer
   * into the InclinometerImpl.
   *
   * @param data The new inclinometer reading
   */
  void 
  InclinometerImpl::integrateData(const InclinometerEventIDL& _data)
  {
    assert(_data.inclination.pitch > (float)-M_PI &&
	   _data.inclination.pitch <= (float)M_PI);
    assert(_data.inclination.roll > (float)-M_PI &&
	   _data.inclination.roll <= (float)M_PI);

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
  InclinometerImpl::cancel() {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  InclinometerEventIDL
  InclinometerImpl::getInclination() throw()
  {
    Guard guard(mutex_);
    return data_;
  }

  InclinometerEventIDL
  InclinometerImpl::getWaitInclination() throw(ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw ETimeOut();

    return data_;
  }

  void
  InclinometerImpl::getMinMaxPitch(CORBA::Float& minPitch,
				   CORBA::Float& maxPitch) throw()
  {
    minPitch = params_.minPitch;
    maxPitch = params_.maxPitch;
  }

  void
  InclinometerImpl::getMinMaxRoll(CORBA::Float& minRoll,
				  CORBA::Float& maxRoll) throw()
  {
    minRoll = params_.minRoll;
    maxRoll = params_.maxRoll;
  }
}
