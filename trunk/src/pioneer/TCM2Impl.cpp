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

#include "TCM2Impl.h"

#include "miro/TimeHelper.h"
#include "miro/StructuredPushSupplier.h"

#include <cmath>

namespace Pioneer
{

  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value TCM2Dispatcher::maxWait_(0, 100000);

  TCM2Dispatcher::TCM2Dispatcher(Miro::StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("TCM2");
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
  TCM2Dispatcher::setData(const Miro::TCM2EventIDL& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int
  TCM2Dispatcher::svc()
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
  TCM2Dispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void
  TCM2Dispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Miro::Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }


  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value TCM2Impl::maxWait_(1, 0);

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
  TCM2Impl::TCM2Impl(const TCM2Parameters& _params,
			     Miro::StructuredPushSupplier * _supplier,
			     bool _asynchDispatching) :
    CompassImpl(_supplier, _asynchDispatching),
    InclinometerImpl(_params.inclinometer, _supplier, _asynchDispatching),
    MagnetometerImpl(_params.magnetometer, _supplier, _asynchDispatching),
    ThermometerImpl(_params.thermometer, _supplier, _asynchDispatching),
    params_(_params),
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_),
    asynchDispatching_(_asynchDispatching),
    dispatcherThread_(_supplier)
  {
    data_.time.sec = 0;
    data_.time.usec = 0;
    data_.heading = NAN; // uninitialized
    data_.inclination.pitch = NAN; // uninitialized
    data_.inclination.roll = NAN; // uninitialized
    data_.fieldstrength.x = NAN; // uninitialized
    data_.fieldstrength.y = NAN; // uninitialized
    data_.fieldstrength.z = NAN; // uninitialized
    data_.temperature = NAN; // uninitialized

    if (asynchDispatching_)
      dispatcherThread_.detach(1);
  }

  // Implementation skeleton destructor
  TCM2Impl::~TCM2Impl()
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
  void
  TCM2Impl::integrateData(const Miro::TCM2EventIDL& _data)
  {
    assert(_data.heading > (float)-M_PI &&
	   _data.heading <= (float)M_PI);
    assert(_data.inclination.pitch > (float)-M_PI &&
	   _data.inclination.pitch <= (float)M_PI);
    assert(_data.inclination.roll > (float)-M_PI &&
	   _data.inclination.roll <= (float)M_PI);

    { // scope for guard
      Miro::Guard guard(mutex_);

      // Copy the new data
      data_ = _data;

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

    // call the base four classes' integrateData methods as well
    Miro::CompassEventIDL compassdata;
    compassdata.time = _data.time;
    compassdata.heading = _data.heading;
    Miro::CompassImpl::integrateData(compassdata);

    Miro::InclinometerEventIDL inclinometerdata;
    inclinometerdata.time = _data.time;
    inclinometerdata.inclination = _data.inclination;
    Miro::InclinometerImpl::integrateData(inclinometerdata);

    Miro::MagnetometerEventIDL magnetometerdata;
    magnetometerdata.time = _data.time;
    magnetometerdata.fieldstrength = _data.fieldstrength;
    Miro::MagnetometerImpl::integrateData(magnetometerdata);

    Miro::ThermometerEventIDL thermometerdata;
    thermometerdata.time = _data.time;
    thermometerdata.temperature = _data.temperature;
    Miro::ThermometerImpl::integrateData(thermometerdata);
  }

  void
  TCM2Impl::cancel() {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  Miro::TCM2EventIDL
  TCM2Impl::getData() throw()
  {
    Miro::Guard guard(mutex_);
    return data_;
  }

  Miro::TCM2EventIDL
  TCM2Impl::getWaitData() throw(Miro::ETimeOut)
  {
    Miro::Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();

    return data_;
  }

  void
  TCM2Impl::integrateData(const Miro::CompassEventIDL& _data)
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
  TCM2Impl::integrateData(const Miro::InclinometerEventIDL& _data)
  {
    { // scope for guard
      Miro::Guard guard(mutex_);

      // Copy the new data
      data_.time = _data.time;
      data_.inclination = _data.inclination;

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

    Miro::InclinometerImpl::integrateData(_data);
  }

  void
  TCM2Impl::integrateData(const Miro::MagnetometerEventIDL& _data)
  {
    { // scope for guard
      Miro::Guard guard(mutex_);

      // Copy the new data
      data_.time = _data.time;
      data_.fieldstrength = _data.fieldstrength;

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

    Miro::MagnetometerImpl::integrateData(_data);
  }

  void
  TCM2Impl::integrateData(const Miro::ThermometerEventIDL& _data)
  {
    { // scope for guard
      Miro::Guard guard(mutex_);

      // Copy the new data
      data_.time = _data.time;
      data_.temperature = _data.temperature;

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

    Miro::ThermometerImpl::integrateData(_data);
  }
};
