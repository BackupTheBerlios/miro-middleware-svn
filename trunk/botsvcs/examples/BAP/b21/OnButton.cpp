// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "OnButton.h"

#include "idl/B21ButtonsC.h"
#include "miro/IO.h"

const std::string OnButton::name_ = "OnButton";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(OnButton, OnButtonParameters)

OnButton::OnButton(CosNotifyChannelAdmin::EventChannel_ptr _ec,
		   const std::string& _domainName) :
  Super(_ec)
{
  setSingleSubscription(_domainName, "Button");
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
    std::cerr << "OnButton: No ButtonStatusIDL message." << std::endl;
  }
}

const std::string&
OnButton::getBehaviourName() const 
{
  return name_;
}
