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
