// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
// 
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
Timer::init(const Miro::BehaviourParameters* timerParameters) 
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
