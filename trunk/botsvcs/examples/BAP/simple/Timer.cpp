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
#include "Timer.h"

#include <ace/OS.h>

// this is a polling implementation
// TODO: make it use the ACE_Reactor stuff

const std::string Timer::name_ = "Timer";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(Timer, TimerParameters);

Timer::Timer(ACE_Reactor &ar_) :
  TimedBehaviour(ar_)
{
}

void 
Timer::init(const Miro::BAP::BehaviourParameters* timerParameters) 
{
  Super::init(timerParameters);

  // prevent concurrent access from action()
  Miro::Guard guard(mutex_);
  // initialize the timer
  timeValue_ = ACE_OS::gettimeofday();
}

void 
Timer::action()
{
  bool transition = false;

  // get the parameters
  const TimerParameters * params =
    dynamic_cast<const TimerParameters *>(params_);
  // make sure nothing went wrong
  assert(params != NULL);

  // prevent concurrent access from init()
  mutex_.acquire();

  ACE_Time_Value atv = ACE_OS::gettimeofday() - timeValue_;
  if (atv > params->time) {
    transition = true;
  }

  // we must not hold the mutex on sendMessage()
  // a transition calls init() so if Timer is
  // active in the next behaviour we get stuck
  mutex_.release();

  // send a transition on timeout
  if (transition)
    sendMessage("Timeout");
}

const std::string&
Timer::getBehaviourName() const {
  return name_;
}
