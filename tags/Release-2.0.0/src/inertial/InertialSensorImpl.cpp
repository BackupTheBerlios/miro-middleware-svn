// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2006
// Institute for Softwaretechnology, Technical University Graz, Austria
//
// $Id: InertialSensorImpl_cpp,v 1.0 2006/02/28 11:59:41 limberger Exp $
//
//////////////////////////////////////////////////////////////////////////////


#include "InertialSensorImpl.h"

#include "miro/TimeHelper.h"
#include "miro/StructuredPushSupplier.h"

#include <cmath>

#if GCC_MAJOR_VERSION == 2
#if GCC_MINOR_VERSION == 95
#include <bits/nan.h>
#endif
#endif

namespace Miro
{

  class StructuredPushSupplier;

  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value InertialSensorDispatcher::maxWait_(0, 100000);

  InertialSensorDispatcher::InertialSensorDispatcher(Miro::StructuredPushSupplier * _supplier) :
    supplier_( _supplier ),
    mutex_( ),
    cond_( mutex_ )
  {
    if (supplier_) {
      std::cout << "InertialSensorDispatcher" << std::endl;
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name =
        CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name =
        CORBA::string_dup("Inertial");
      notifyEvent_.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent_.header.variable_header.length(0);   // put nothing here
      notifyEvent_.filterable_data.length(0);          // put nothing here

      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent_.header.fixed_header.event_type;
      supplier_->addOffers(offers);
    }
  }

  void InertialSensorDispatcher::setData(const Miro::InertialSensorEventIDL& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int InertialSensorDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous InertialSensor dispatching");

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

  void InertialSensorDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void InertialSensorDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Miro::Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }

}

namespace Inertial
{

  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value InertialSensorImpl::maxWait_(1, 0); /*!!!*/

  // *******************************************************************************************************************
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
  InertialSensorImpl::InertialSensorImpl(const InertialParameters& _parameters,
                             Miro::StructuredPushSupplier * _supplier,
                             bool _asynchDispatching) :
    Miro::CompassImpl(_supplier, _asynchDispatching),
    Miro::InclinometerImpl(_parameters.inclinometer, _supplier, _asynchDispatching),
    parameters_(_parameters),
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_),
    asynchDispatching_(_asynchDispatching),
    dispatcherThread_(_supplier)
  {
    std::cout << supplier_ << std::endl;
    data_.time.sec = 0;
    data_.time.usec = 0;
    data_.inertial_data.pitch = NAN; // uninitialized
    data_.inertial_data.roll = NAN; // uninitialized
    data_.inertial_data.yaw  = NAN; // uninitialized

    if (asynchDispatching_)
    {
      // DEBUG
      //std::cout << "Dispatch einrichten: " << std::endl;
      dispatcherThread_.detach(1);
    }
  }

  // Implementation skeleton destructor
  InertialSensorImpl::~InertialSensorImpl()
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
  void InertialSensorImpl::integrateData(const Miro::InertialSensorEventIDL& _data)
  {
    assert(_data.inertial_data.pitch > -M_PI &&
           _data.inertial_data.pitch <= M_PI);
    assert(_data.inertial_data.roll > -M_PI &&
           _data.inertial_data.roll <= M_PI);
    assert(_data.inertial_data.yaw > -M_PI &&
           _data.inertial_data.yaw <= M_PI);
    { // scope for guard
      Miro::Guard guard(mutex_);

      // Copy the new data
      data_ = _data;

      cond_.broadcast();
    }
    //std::cout << data_.inertial_data.yaw << std::endl;
    // send events
    if ( supplier_ )
    {
      if ( asynchDispatching_ )
      {
        Miro::Guard guard( dispatcherThread_.mutex_ );
        dispatcherThread_.setData( data_ );
        dispatcherThread_.cond_.broadcast( );
      }
      else
      {
        dispatcherThread_.setData( data_ );
        dispatcherThread_.dispatch( );
      }
    }

    // call the base classes' integrateData methods as well
    Miro::CompassEventIDL compassdata;
    compassdata.time = data_.time;
    compassdata.heading = data_.inertial_data.yaw;
    Miro::CompassImpl::integrateData(compassdata);

    Miro::InclinometerEventIDL inclinometerdata;
    inclinometerdata.time = data_.time;
    inclinometerdata.inclination.pitch = data_.inertial_data.pitch;
    inclinometerdata.inclination.roll  = data_.inertial_data.roll;
    Miro::InclinometerImpl::integrateData(inclinometerdata);
  }

  void InertialSensorImpl::cancel()
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  Miro::InertialSensorEventIDL InertialSensorImpl::getInertialData() throw()
  {
    Miro::Guard guard(mutex_);
    return data_;
  }

  Miro::InertialSensorEventIDL InertialSensorImpl::getWaitInertialData() throw(Miro::ETimeOut)
  {
    Miro::Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw Miro::ETimeOut();

    return data_;
  }

}
