// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#include "OnButton.h"

#include "idl/B21ButtonsC.h"
#include "miro/IO.h"

const std::string OnButton::name_ = "OnButton";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(OnButton, OnButtonParameters)

OnButton::OnButton(CosNotifyChannelAdmin::EventChannel_ptr _ec,
		   const string& _domainName) :
  Super(_ec)
{
  // subscribe for Button events
  CosNotification::EventTypeSeq added;
  CosNotification::EventTypeSeq removed;
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(_domainName.c_str());
  added[0].type_name = CORBA::string_dup("Button");

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumer.consumerAdmin_->subscription_change(added, removed);
}

void
OnButton::action()
{
  // get parameters
  const OnButtonParameters * params =
    dynamic_cast<const OnButtonParameters *>(params_);
  // make sure nothing went wrong
  assert(params != NULL);

  // pointer to button message event type
  const Miro::ButtonStatusIDL * pButtonStatus;
  if (event->remainder_of_body >>= pButtonStatus) {
    // if it is the desired button
    if (pButtonStatus->event == 
	((params->onPress)? Miro::Button::ON_PRESS : Miro::Button::ON_RELEASE) &&
	pButtonStatus->number == params->button) {

      // send transition
      sendMessage("Button");
    }
  }
  else {
    // somthing went seriously wrong
    std::cerr << "OnButton: No ButtonStatusIDL message." << endl;
  }
}

const string&
OnButton::getBehaviourName() const 
{
  return name_;
}
