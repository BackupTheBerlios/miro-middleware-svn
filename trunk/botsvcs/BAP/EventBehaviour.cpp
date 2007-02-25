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
#include "EventBehaviour.h"

#include "miro/Log.h"

namespace Miro
{
  namespace BAP
  {
    BehaviourEventConsumer::BehaviourEventConsumer(EventBehaviour& _behaviour,
						   CosNotifyChannelAdmin::EventChannel_ptr _ec) :
      Super(_ec),
      behaviour_(_behaviour)
    {
      MIRO_LOG_CTOR("BehaviourEventConsumer");
    }

    void
    BehaviourEventConsumer::push_structured_event(const CosNotification::StructuredEvent & notification)
      throw(CORBA::SystemException, CosEventComm::Disconnected)
    {
      // we can suspend messages by now
      // but this check makes us a bit less racy
      if (!behaviour_.active_) 
	return;

      // clean up dangling behaviour parameters
      // left over from ActionPattern::setBehaviourParameters()

      delete behaviour_.oldParams_;
      behaviour_.oldParams_ = NULL;

      // make event visible to the behaviour
      behaviour_.event = &notification;
      // call its action method
      behaviour_.action();
    }


    EventBehaviour::EventBehaviour(CosNotifyChannelAdmin::EventChannel_ptr _ec) :
      Super(),
      event(NULL),
      consumer(*this, _ec)
    {
      MIRO_LOG_CTOR("EventBehaviour");
    }

    EventBehaviour::~EventBehaviour()
    {
      MIRO_LOG_DTOR("EventBehaviour");
    }

    void
    EventBehaviour::disconnect()
    {
      consumer.disconnect();
    }

    void
    EventBehaviour::open()
    {
      Super::open();
      consumer.subscribe();
    }

    void
    EventBehaviour::close()
    {
      consumer.unsubscribe();
      Super::close();
    }

    void
    EventBehaviour::setSingleSubscription(std::string const& _domainName,
					  std::string const& _eventName)
    {
      CosNotification::EventTypeSeq added;
      added.length(1);
      
      added[0].domain_name =  CORBA::string_dup(_domainName.c_str());
      added[0].type_name = CORBA::string_dup(_eventName.c_str());
      
      consumer.setSubscriptions(added, active_);
    }
  }
}
