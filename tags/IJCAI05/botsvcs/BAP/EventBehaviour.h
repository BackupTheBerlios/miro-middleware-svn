// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
