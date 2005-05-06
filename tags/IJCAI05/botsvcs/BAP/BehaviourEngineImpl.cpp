// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BehaviourEngineImpl.h"
#include "ActionPattern.h"

namespace Miro
{
  namespace BAP
  {
    // Implementation skeleton constructor
    BehaviourEngineImpl::BehaviourEngineImpl(StructuredPushSupplier * _pSupplier,
					     bool _openOnLoad) :
      policy_(_pSupplier),
      openOnLoad_(_openOnLoad),
      suspendedPattern_(NULL)
    {
    }
  
    // Implementation skeleton destructor
    BehaviourEngineImpl::~BehaviourEngineImpl()
    {
    }
  
    void
    BehaviourEngineImpl::loadPolicy(const char * policy)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EMalformedXML,
		       Miro::BehaviourEngine::EMalformedPolicy))
    {
      Guard guard(mutex_);

      policy_.clear();
      Policy::loadPolicy(policy_, policy);
      if (openOnLoad_)
	policy_.open();
    }
  
    void
    BehaviourEngineImpl::loadPolicyFile(const char * fileName)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EMalformedXML,
		       Miro::BehaviourEngine::EMalformedPolicy,
		       Miro::BehaviourEngine::EFile))
    {
      Guard guard(mutex_);

      policy_.clear();
      suspendedPattern_ = NULL;

      Policy::loadPolicyFile(policy_, fileName);
      if (openOnLoad_)
	policy_.open(NULL, std::string());
    }
  
    void 
    BehaviourEngineImpl::openPolicy()
      ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy))
    {
      Guard guard(mutex_);
      if (!policy_.valid())
	throw Miro::BehaviourEngine::ENoPolicy();

      suspendedPattern_ = NULL;
      policy_.open(NULL, std::string());
    }
  
    void
    BehaviourEngineImpl::closePolicy ()
      ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy))
    {
      Guard guard(mutex_);
      if (!policy_.valid())
	throw Miro::BehaviourEngine::ENoPolicy();

      suspendedPattern_ = NULL;
      policy_.close();
    }

    void 
    BehaviourEngineImpl::suspendPolicy()
      ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy))
    {
      Guard guard(mutex_);
      Guard guard2(policy_.transitionMutex());
      suspendedPattern_ = policy_.currentActionPattern();
      policy_.close();
    }
  
    void
    BehaviourEngineImpl::resumePolicy ()
      ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy))
    {
      Guard guard(mutex_);
      if (!policy_.valid())
	throw Miro::BehaviourEngine::ENoPolicy();

      if (suspendedPattern_) {
	std::string pattern = suspendedPattern_->getName();
	policy_.openPattern(pattern);
	suspendedPattern_ = NULL;
      }
    }

    void
    BehaviourEngineImpl::openActionPattern(const char * pattern) 
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EUnknownActionPattern))
    {
      policy_.openPattern(std::string(pattern));
    }
    
    void
    BehaviourEngineImpl::sendTransition(const char * transition) ACE_THROW_SPEC (())
    {
      Guard guard2(policy().transitionMutex());
      ActionPattern * pattern = policy_.currentActionPattern();
      if (pattern)
	pattern->protectedSendTransitionMessage(std::string(transition));
    }

    void
    BehaviourEngineImpl::sendTransitionFromActionPattern(const char * pattern,
							 const char * transition)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EUnknownActionPattern,
		       Miro::BehaviourEngine::EUnregisteredTransition))
    {
      Pattern * pPattern = policy_.getPattern(std::string(pattern));
      if (pPattern) {
	pPattern->sendTransitionMessage(std::string(transition));
      }
      else
	throw Miro::BehaviourEngine::EUnknownActionPattern();
    }
  }
}
