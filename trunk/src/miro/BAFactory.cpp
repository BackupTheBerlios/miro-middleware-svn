// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
// 
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BAFactory.h"

#include <miro/TimedBehaviour.h>
#include <miro/EventBehaviour.h>
#include <miro/TaskBehaviour.h>

#include <miro/BehaviourRepository.h>
#include <miro/ArbiterRepository.h>

namespace Miro
{
  BAFactory::BAFactory()
  {
  }

  BAFactory::~BAFactory()
  {
    {
      TimedBehaviourVector::iterator first, last = timedBehaviours_.end();
      for (first = timedBehaviours_.begin(); first != last; ++first)
	delete *first;
    }

    {
      EventBehaviourVector::iterator first, last = eventBehaviours_.end();
      for (first = eventBehaviours_.begin(); first != last; ++first)
	delete *first;
    }

    {
      TaskBehaviourVector::iterator first, last = taskBehaviours_.end();
      for (first = taskBehaviours_.begin(); first != last; ++first)
	delete *first;
    }

    {
      ArbiterVector::iterator first, last = arbiters_.end();
      for (first = arbiters_.begin(); first != last; ++first)
	delete *first;
    }
  }

  void
  BAFactory::init()
  {
    BehaviourRepository * br = BehaviourRepository::instance();
    ArbiterRepository * ar = ArbiterRepository::instance();

    {
      TimedBehaviourVector::iterator first, last = timedBehaviours_.end();
      for (first = timedBehaviours_.begin(); first != last; ++first)
	br->registerBehaviour((*first));
    }
    
    {
      EventBehaviourVector::iterator first, last = eventBehaviours_.end();
      for (first = eventBehaviours_.begin(); first != last; ++first)
	br->registerBehaviour((*first));
    }

    {
      TaskBehaviourVector::iterator first, last = taskBehaviours_.end();
      for (first = taskBehaviours_.begin(); first != last; ++first)
	br->registerBehaviour((*first));
    }

    {
      ArbiterVector::iterator first, last = arbiters_.end();
      for (first = arbiters_.begin(); first != last; ++first)
	ar->registerArbiter((*first));
    }
  }

  void
  BAFactory::connect()
  {
    /*
      EventBehaviourVector::iterator first, last = eventBehaviours_.end();
      for (first = eventBehaviours_.begin(); first != last; ++first)
      (*first)->connect();
    */
  }

  void
  BAFactory::disconnect()
  {
    EventBehaviourVector::iterator first, last = eventBehaviours_.end();
    for (first = eventBehaviours_.begin(); first != last; ++first)
      (*first)->disconnect();
  }
};  
