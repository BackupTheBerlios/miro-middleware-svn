// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
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

    /** StructuredPushSupplier methods */
    void push_structured_event(const CosNotification::StructuredEvent & notification)
      throw(CORBA::SystemException, CosEventComm::Disconnected);

  protected:
    EventBehaviour& behaviour_;
  };

  class EventBehaviour : 
    public Behaviour
  {
    typedef Behaviour Super;

  public:
    EventBehaviour(CosNotifyChannelAdmin::EventChannel_ptr _ec);
    virtual ~EventBehaviour();

    void open();
    void close();

    void disconnect();

  protected:
    BehaviourEventConsumer consumer;
    CosNotification::StructuredEvent const * event;

    friend class BehaviourEventConsumer;
  };
};
#endif
