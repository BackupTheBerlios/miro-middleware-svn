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

#include "TimedBehaviour.h"
#include "TimeHelper.h"

namespace Miro
{
  BEHAVIOUR_PARAMETERS_FACTORY_IMPL(TimedBehaviour, TimedBehaviourParameters)

  TimedBehaviour::TimedBehaviour(ACE_Reactor &ar_):
    // store the ace reactor
    reactor(ar_), 
    timerId(0)
  {
  }

  void
  TimedBehaviour::init(const BehaviourParameters* _params) 
  {
    Super::init(_params);

    if (active_) {
      const TimedBehaviourParameters * params = 
	dynamic_cast<const TimedBehaviourParameters *>(params_);
      reactor.reset_timer_interval(timerId, params->pace);
    }
  }

  void
  TimedBehaviour::open() 
  {
    Behaviour::open();
    // register for timer events
    const TimedBehaviourParameters * params = 
      dynamic_cast<const TimedBehaviourParameters *>(params_);
    timerId = reactor.schedule_timer(this, 
				     0, 
				     ACE_Time_Value(0,0), 
				     params->pace);
  }

  void
  TimedBehaviour::close() 
  {
    // unregister for timer events
    reactor.cancel_timer(timerId);
    timerId = 0;

    Behaviour::close();
  }

  int
  TimedBehaviour::handle_timeout (const ACE_Time_Value &, const void*) 
  {
    // clean up dangling behaviour parameters
    // left over from ActionPattern::setBehaviourParameters()    
    delete oldParams_;
    oldParams_ = NULL;

    action();
    return 0;
  }
}
