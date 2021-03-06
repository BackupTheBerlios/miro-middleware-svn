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
#include "AliveEventHandler.h"
#include "AliveCollector.h"
#include "SparrowConnection2003.h"

#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

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

    if (!(aliveIDL.MotorAlive = collector_->motorAlive()))
      MIRO_LOG(LL_ERROR, "Sparrow2003: motor board not alive!");
    if (!(aliveIDL.KickerAlive = collector_->kickAlive()))
      MIRO_LOG(LL_ERROR, "Sparrow2003: kicker board not alive!");
    if (!(aliveIDL.Infrared1Alive = collector_->infrared1Alive()))
      MIRO_LOG(LL_ERROR, "Sparrow2003: infrared board not alive!");
    if (!(aliveIDL.Infrared2Alive = collector_->infrared2Alive()))
      MIRO_LOG(LL_ERROR, "Sparrow2003: infrared board 2 not alive!");
    if (!(aliveIDL.PanAlive = collector_->panAlive()))
      MIRO_LOG(LL_ERROR, "Sparrow2003: pan board not alive!");

    connection_->writeHostAlive();
    
    Miro::Guard guard(dispatcher_.mutex_);
    dispatcher_.setData(aliveIDL);
    dispatcher_.cond_.broadcast();

    return 0;
  }
}

