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


#include "EventBehaviour.h"

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Miro
{

  BehaviourEventConsumer::BehaviourEventConsumer(EventBehaviour& _behaviour,
						 CosNotifyChannelAdmin::EventChannel_ptr _ec) :
    Super(_ec),
    behaviour_(_behaviour)
  {
    DBG(cout << "Constructing BehaviourEventConsumer" << endl);
    // proxySupplier_->suspend_connection();
  }

  void
  BehaviourEventConsumer::push_structured_event(const CosNotification::StructuredEvent & notification)
    throw(CORBA::SystemException, CosEventComm::Disconnected)
  {
    // we can't suspend messages by now
    if (!behaviour_.active_) 
      return;

    // clean up dangling behaviour parameters
    // left over from ActionPattern::setBehaviourParameters()    
    //    delete behaviour_.oldParams_;
    //    behaviour_.oldParams_ = NULL;

    behaviour_.event = &notification;
    behaviour_.action();
  }


  EventBehaviour::EventBehaviour(CosNotifyChannelAdmin::EventChannel_ptr _ec) :
    Super(),
    consumer(*this, _ec),
    event(NULL)
  {
    DBG(cout << "Constructing EventBehaviour" << endl);
  }

  EventBehaviour::~EventBehaviour()
  {
    DBG(cout << "Destructing EventBehaviour" << endl);
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
    // consumer.proxySupplier_->resume_connection();
  }

  void
  EventBehaviour::close()
  {
    // consumer.proxySupplier_->suspend_connection();
    Super::close();
  }
};
