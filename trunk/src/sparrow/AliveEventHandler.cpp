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

#include <idl/SparrowAliveC.h>

namespace Sparrow
{
  AliveEventHandler::AliveEventHandler(AliveCollector * _collector, 
				       Connection2003 * _connection,
				       Miro::StructuredPushSupplier * _pSupplier) :
    Super(),
    connection_(_connection),
    collector_(_collector),
    pSupplier_(_pSupplier)
  {
    if (pSupplier_) {
      notifyEvent_.header.fixed_header.event_type.domain_name =
        CORBA::string_dup(pSupplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name =
	CORBA::string_dup("SparrowAlive");
      notifyEvent_.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent_.header.variable_header.length(0);   // put nothing here
      notifyEvent_.filterable_data.length(0);          // put nothing here

      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent_.header.fixed_header.event_type;
      pSupplier_->addOffers(offers);
    }
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
    
    notifyEvent_.remainder_of_body <<= aliveIDL;
    if(pSupplier_)
      pSupplier_->sendEvent(notifyEvent_);
    
    return 0;
  }
}

