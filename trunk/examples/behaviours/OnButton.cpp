// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#include "OnButton.h"

#include "miro/ButtonsC.h"
#include "miro/IO.h"

using std::cout;
using std::cerr;
using std::string;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;

using Miro::Button;
using Miro::StructuredPushConsumer;

string OnButton::name_ = "OnButton";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(OnButton, OnButtonParameters)

OnButton::OnButton(CosNotifyChannelAdmin::EventChannel_ptr _ec,
		   const string& _domainName) :
  Super(_ec),
  domainName_(_domainName)
{
  cout << "Constructing OnButton behaviour" << endl;

  EventTypeSeq added(1);
  EventTypeSeq removed(1);
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName_.c_str());
  added[0].type_name = CORBA::string_dup("B21ButtonStatusIDL");

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumer.consumerAdmin_->subscription_change(added, removed);
}

void
OnButton::action()
{
  cout << "OnButton::action called." << endl;
  const Miro::ButtonStatusIDL * pButtonStatus;
  const OnButtonParameters * params =
    dynamic_cast<const OnButtonParameters *>(params_);

  if (event->remainder_of_body >>= pButtonStatus) {
    cout << "ButtonStatus - time:" << pButtonStatus->time 
	 << " id:" << pButtonStatus->number 
	 << " event:" << pButtonStatus->event << endl;
    if (pButtonStatus->event == 
	((params->onPress)? Button::ON_PRESS : Button::ON_RELEASE) &&
	(long)pButtonStatus->number == params->button) {

      cout << "\a" << endl;
      sendMessage("Button");
      cout << "button message" << endl;
    }
  }
  else
    cerr << "OnButton: No ButtonStatusIDL message." << endl;
}

const string&
OnButton::getBehaviourName() const 
{
  return name_;
}
