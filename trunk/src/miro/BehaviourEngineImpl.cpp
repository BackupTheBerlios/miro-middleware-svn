// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BehaviourEngineImpl.h"
#include "ActionPattern.h"

namespace Miro
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
    policy_.loadPolicy(policy);
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

    policy_.loadPolicyFile(fileName);
    if (openOnLoad_)
      policy_.open();
  }
  
  void 
  BehaviourEngineImpl::openPolicy()
    ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy))
  {
    Guard guard(mutex_);
    if (!policy_.valid())
      throw Miro::BehaviourEngine::ENoPolicy();

    suspendedPattern_ = NULL;
    policy_.open();
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
      std::string pattern = suspendedPattern_->getActionPatternName();
      policy_.openActionPattern(pattern);
      suspendedPattern_ = NULL;
    }
  }

  void
  BehaviourEngineImpl::openActionPattern(const char * pattern) 
    ACE_THROW_SPEC ((Miro::BehaviourEngine::EUnknownActionPattern))
  {
    policy_.openActionPattern(std::string(pattern));
  }
    
  void
  BehaviourEngineImpl::sendTransition(const char * transition) ACE_THROW_SPEC (())
  {
    ActionPattern * pattern = policy_.currentActionPattern();
    if (pattern)
      pattern->sendMessage(NULL, std::string(transition));
  }

  void
  BehaviourEngineImpl::sendTransitionFromActionPattern(const char * pattern,
						       const char * transition)
    ACE_THROW_SPEC ((Miro::BehaviourEngine::EUnknownActionPattern,
		     Miro::BehaviourEngine::EUnregisteredTransition))
  {
    ActionPattern * pPattern = policy_.getActionPattern(std::string(pattern));
    if (pPattern) {
      pPattern->sendMessage(NULL, std::string(transition));
    }
    else
      throw Miro::BehaviourEngine::EUnknownActionPattern();
  }
};
