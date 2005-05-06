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
#include "TactileStop.h"

#include "botsvcs/BAP/MotionArbiterMessage.h"

using std::endl;

const std::string TactileStop::name_ = "TactileStop";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(TactileStop, TactileStopParameters);

TactileStop::TactileStop(Miro::Motion_ptr _motion,
			 CosNotifyChannelAdmin::EventChannel_ptr _ec,
			 const std::string& _domainName) :
  Super(_ec),
  motion_(Miro::Motion::_duplicate(_motion))
{
  MIRO_LOG_CTOR("TactileStop");

  // Tell the event channel to push us Tactile events
  setSingleSubscription(_domainName, "Tactile");
}

TactileStop::~TactileStop()
{
  MIRO_LOG_DTOR("TactileStop");
}

void
TactileStop::action()
{
  // get control over the arbiter
  // halt motors;
  Miro::BAP::MotionArbiterMessage message;

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
