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
#include "BAFactory.h"
#include "TimedBehaviour.h"
#include "EventBehaviour.h"
#include "TaskBehaviour.h"

#include "BehaviourRepository.h"
#include "ArbiterRepository.h"

namespace Miro
{
  namespace BAP
  {
    BAFactory::BAFactory()
    {
    }

    BAFactory::~BAFactory()
    {
      fini();
    }

    void
    BAFactory::init()
    {
      BehaviourRepository * br = BehaviourRepository::instance();
      ArbiterRepository * ar = ArbiterRepository::instance();

      {
	TimedBehaviourVector::iterator first, last = timedBehaviours_.end();
	for (first = timedBehaviours_.begin(); first != last; ++first)
	  br->add((*first)->getName(), (*first));
      }
    
      {
	EventBehaviourVector::iterator first, last = eventBehaviours_.end();
	for (first = eventBehaviours_.begin(); first != last; ++first)
	  br->add((*first)->getName(), (*first));
      }

      {
	TaskBehaviourVector::iterator first, last = taskBehaviours_.end();
	for (first = taskBehaviours_.begin(); first != last; ++first)
	  br->add((*first)->getName(), (*first));
      }

      {
	ArbiterVector::iterator first, last = arbiters_.end();
	for (first = arbiters_.begin(); first != last; ++first)
	  ar->add((*first)->getName(), (*first));
      }
    }

    void
    BAFactory::fini()
    {
      BehaviourRepository * br = BehaviourRepository::instance();
      ArbiterRepository * ar = ArbiterRepository::instance();

      {
	TimedBehaviourVector::iterator first, last = timedBehaviours_.end();
	for (first = timedBehaviours_.begin(); first != last; ++first)
	  br->remove((*first)->getName());
	timedBehaviours_.clear();
      }
    
      {
	EventBehaviourVector::iterator first, last = eventBehaviours_.end();
	for (first = eventBehaviours_.begin(); first != last; ++first)
	  br->remove((*first)->getName());
	eventBehaviours_.clear();
      }

      {
	TaskBehaviourVector::iterator first, last = taskBehaviours_.end();
	for (first = taskBehaviours_.begin(); first != last; ++first)
	  br->remove((*first)->getName());
	taskBehaviours_.clear();
      }

      {
	ArbiterVector::iterator first, last = arbiters_.end();
	for (first = arbiters_.begin(); first != last; ++first)
	  ar->remove((*first)->getName());
	arbiters_.clear();
      }
    }

    void
    BAFactory::connect()
    {
      //      EventBehaviourVector::iterator first, last = eventBehaviours_.end();
      //      for (first = eventBehaviours_.begin(); first != last; ++first)
      //	(*first)->connect();
    }

    void
    BAFactory::disconnect()
    {
      EventBehaviourVector::iterator first, last = eventBehaviours_.end();
      for (first = eventBehaviours_.begin(); first != last; ++first)
	(*first)->disconnect();
    }

    void
    BAFactory::add(TimedBehaviour * _behaviour)
    {
      timedBehaviours_.push_back(_behaviour);
    }

    void
    BAFactory::add(EventBehaviour * _behaviour)
    {
      eventBehaviours_.push_back(_behaviour);
    }

    void
    BAFactory::add(TaskBehaviour * _behaviour)
    {
      taskBehaviours_.push_back(_behaviour);
    }

    void
    BAFactory::add(Arbiter * _behaviour)
    {
      arbiters_.push_back(_behaviour);
    }
  }
}
