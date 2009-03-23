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
#ifndef BAP_EventBehaviour_h
#define BAP_EventBehaviour_h

#include "Behaviour.h"

#include "miro/StructuredPushConsumer.h"

namespace Miro
{
  namespace BAP
  {
    // forward declaration
    class EventBehaviour;

    class BehaviourEventConsumer :   public Miro::StructuredPushConsumer
    {
      //! Super class type.
      typedef Miro::StructuredPushConsumer Super;
      
    public:
      //! Initializing constructor.
      BehaviourEventConsumer(EventBehaviour& _behaviour,
			     CosNotifyChannelAdmin::EventChannel_ptr _ec);
      
      //! StructuredPushSupplier inherited method.
      void push_structured_event(const CosNotification::StructuredEvent & notification)
	throw(CORBA::SystemException, CosEventComm::Disconnected);

    protected:
      //! Reference to the corresponding behaviour.
      EventBehaviour& behaviour_;
    };

    class EventBehaviour : public Behaviour
    {
      //! Super class type.
      typedef Behaviour Super;
      
    public:
      //! Initializing constructor.
      EventBehaviour(CosNotifyChannelAdmin::EventChannel_ptr _ec);
      virtual ~EventBehaviour();
      
      void open();
      void close();
      
      void disconnect();

      void setSingleSubscription(std::string const& _domain_name,
				 std::string const& _event_name);
      void setSubscriptions(CosNotification::EventTypeSeq const& _newSubscriptions);

    protected:
      CosNotification::StructuredEvent const * event;
      
      friend class BehaviourEventConsumer;

    private:
      BehaviourEventConsumer consumer;
    };

    inline
    void
    EventBehaviour::setSubscriptions(CosNotification::EventTypeSeq const& _newSubscriptions) {
      consumer.setSubscriptions(_newSubscriptions, active_);
    }
  }
}
#endif // BAP_EventBehaviour_h
