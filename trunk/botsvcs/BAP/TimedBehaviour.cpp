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
#include "TimedBehaviour.h"
#include "miro/TimeHelper.h"

namespace Miro
{
  namespace BAP
  {
    BEHAVIOUR_PARAMETERS_FACTORY_IMPL(TimedBehaviour, TimedBehaviourParameters);

    /**
     * @param _reactor Reference to the reactor to register with.
     */
    TimedBehaviour::TimedBehaviour(ACE_Reactor &_reactor):
      // store the ace reactor
      reactor(_reactor), 
      timerId(-1)
    {
      MIRO_LOG_CTOR("TimedBehaviour");
    }

    TimedBehaviour::~TimedBehaviour()
    {
      MIRO_LOG_DTOR("TimedBehaviour");
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
      timerId = -1;

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
}
