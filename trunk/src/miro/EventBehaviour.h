// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef EventBehaviour_h
#define EventBehaviour_h

#include "Behaviour.h"
#include "StructuredPushConsumer.h"

namespace Miro
{
  // forward declaration
  class EventBehaviour;

  class BehaviourEventConsumer :   public Miro::StructuredPushConsumer
  {
    typedef Miro::StructuredPushConsumer Super;

  public:
    BehaviourEventConsumer(EventBehaviour& _behaviour,
			   CosNotifyChannelAdmin::EventChannel_ptr _ec);

    //! StructuredPushConsumer methods
    void push_structured_event(const CosNotification::StructuredEvent & notification)
      throw(CORBA::SystemException, CosEventComm::Disconnected);

  protected:
    EventBehaviour& behaviour_;
  };

  class EventBehaviour :  public Behaviour
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

  protected:
    void setSingleSubscription(std::string const& _domain_name, 
			       std::string const& _event_name);
    void setSubscriptions(CosNotification::EventTypeSeq const& _newSubscriptions);

    CosNotification::StructuredEvent const * event;

  private:
    BehaviourEventConsumer consumer;

    friend class BehaviourEventConsumer;
  };

  inline
  void
  EventBehaviour::setSubscriptions(CosNotification::EventTypeSeq const& _newSubscriptions) {
    consumer.setSubscriptions(_newSubscriptions, active_);
  }
}
#endif
