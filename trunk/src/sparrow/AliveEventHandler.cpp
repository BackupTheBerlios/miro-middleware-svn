// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "AliveEventHandler.h"
#include "AliveCollector.h"
#include "SparrowConnection2003.h"

#include "miro/StructuredPushSupplier.h"

namespace Sparrow
{
  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value AliveDispatcher::maxWait_(1);

  AliveDispatcher::AliveDispatcher(Miro::StructuredPushSupplier * _supplier) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("SparrowAlive");

      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent_.header.fixed_header.event_type;
      supplier_->addOffers(offers);
    }
  }

  void
  AliveDispatcher::setData(Miro::SparrowAliveIDL const& _data)
  {
    notifyEvent_.remainder_of_body <<= _data;
  };

  int
  AliveDispatcher::svc()
  {
    MIRO_LOG(LL_NOTICE, "Asynchronous SparrowAlive dispatching");

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
  AliveDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
  }

  void
  AliveDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    {
      Miro::Guard guard(mutex_);
      cond_.broadcast();
    }
    Super::cancel(_wait);
  }

  AliveEventHandler::AliveEventHandler(AliveCollector * _collector, 
				       Connection2003 * _connection,
				       Miro::StructuredPushSupplier * _pSupplier) :
    Super(),
    connection_(_connection),
    collector_(_collector),
    dispatcher_(_pSupplier)
  {
    dispatcher_.detach(1);
  }

  AliveEventHandler::~AliveEventHandler()
  {
    dispatcher_.cancel();
  }

  int 
  AliveEventHandler::handle_timeout (const ACE_Time_Value &/*current_time*/, 
				     const void */*act*/) 
  {
    Miro::SparrowAliveIDL aliveIDL;

    aliveIDL.MotorAlive = collector_->motorAlive();
    aliveIDL.KickerAlive = collector_->kickAlive();
    aliveIDL.Infrared1Alive = collector_->infrared1Alive();
    aliveIDL.Infrared2Alive = collector_->infrared2Alive();
    aliveIDL.PanAlive = collector_->panAlive();

    connection_->writeHostAlive();
    
    Miro::Guard guard(dispatcher_.mutex_);
    dispatcher_.setData(aliveIDL);
    dispatcher_.cond_.broadcast();

    return 0;
  }
}

