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

using std::cout;
using std::cerr;
using std::string;

using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotification::EventTypeSeq;

using Miro::Motion;
using Miro::Motion_ptr;

namespace 
{
  const string behaviourName("TactileStop");
};

TactileStop::TactileStop(Motion_ptr _motion,
			 EventChannel_ptr _ec,
			 const string& _domainName) :
  Super(_ec),
  motion_(Motion::_duplicate(_motion)),
  domainName_(_domainName)
{
  cout << "Constructing TactileStop behaviour" << endl;

  EventTypeSeq added(1);
  EventTypeSeq removed(1);
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
}

const string&
TactileStop::getBehaviourName() const 
{
  return behaviourName;
}
