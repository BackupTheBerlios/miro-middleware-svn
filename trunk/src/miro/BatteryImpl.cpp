// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BatteryImpl.h"

#include "TimeHelper.h"
#include "StructuredPushSupplier.h"

#include <iostream>

namespace Miro
{

  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value BatteryDispatcher::maxWait_(0, 100000);

  BatteryDispatcher::BatteryDispatcher(StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("Battery");
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
  BatteryDispatcher::setData(const BatteryEventIDL& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int
  BatteryDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous Battery dispatching");

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
  BatteryDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void
  BatteryDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }

  // Implementation skeleton constructor
  BatteryImpl::BatteryImpl(StructuredPushSupplier * _supplier, bool _asynchDispatching) :
    voltage_(-1.),
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_),
    asynchDispatching_(_asynchDispatching),    
    dispatcherThread_(_supplier)
  {    
 	if (asynchDispatching_)
  		dispatcherThread_.detach(1);
  }
  
  // Implementation skeleton destructor
  BatteryImpl::~BatteryImpl()
  {
  }
  
  void
  BatteryImpl::integrateData(double _voltage) ACE_THROW_SPEC(())
  {      
  	
    std::cout << "BatteryImpl::integrateData" << std::endl;
	
    Guard guard(mutex_);

    // Copy the new data
    data_.voltage = _voltage;

    cond_.broadcast();    
	
	// send events
    if (supplier_) 
	{
      if (asynchDispatching_) 
	  {
		Guard guard(dispatcherThread_.mutex_);
		dispatcherThread_.setData(data_);
		dispatcherThread_.cond_.broadcast();
      }
      else 
	  {
		dispatcherThread_.setData(data_);
		dispatcherThread_.dispatch();
      }
    }
    
  }
  
  void BatteryImpl::cancel() 
  {
    if (asynchDispatching_)
      dispatcherThread_.cancel();
  }

  CORBA::Double 
  BatteryImpl::getVoltage() ACE_THROW_SPEC(())
  {
  	Guard guard(mutex_);
    return data_.voltage;
  }
};
