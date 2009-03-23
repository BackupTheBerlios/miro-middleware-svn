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
