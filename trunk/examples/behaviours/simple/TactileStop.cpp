// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#include "TactileStop.h"

#include "miro/MotionArbiterMessage.h"

const std::string TactileStop::name_ = "TactileStop";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(TactileStop, TactileStopParameters);

TactileStop::TactileStop(Miro::Motion_ptr _motion,
			 CosNotifyChannelAdmin::EventChannel_ptr _ec,
			 const std::string& _domainName) :
  Super(_ec),
  motion_(Miro::Motion::_duplicate(_motion)),
  domainName_(_domainName)
{
  // Tell the event channel to push us Tactile events
  
  CosNotification::EventTypeSeq added;
  CosNotification::EventTypeSeq removed;
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName_.c_str());
  added[0].type_name = CORBA::string_dup("Tactile");

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumer.consumerAdmin_->subscription_change(added, removed);
}

void
TactileStop::action()
{
  // get control over the arbiter
  // halt motors;
  Miro::MotionArbiterMessage message;

  message.id = this;
  message.active = true;
  message.velocity.translation = 0;
  message.velocity.rotation = 0;
  arbitrate(message);

  // release motors to allow for manual movement
  motion_->limp();

  // we do not send a transition!
  // after rescue we need a global one!
}

const std::string&
TactileStop::getBehaviourName() const 
{
  return name_;
}
