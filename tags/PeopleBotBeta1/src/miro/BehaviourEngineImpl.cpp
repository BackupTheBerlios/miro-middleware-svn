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

namespace Miro
{
  // Implementation skeleton constructor
  BehaviourEngineImpl::BehaviourEngineImpl(StructuredPushSupplier * _pSupplier) :
    policy_(_pSupplier)
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
  }
  
  void
  BehaviourEngineImpl::loadPolicyFile(const char * fileName)
    ACE_THROW_SPEC ((Miro::BehaviourEngine::EMalformedXML,
		     Miro::BehaviourEngine::EMalformedPolicy,
		     Miro::BehaviourEngine::EFile))
  {
    Guard guard(mutex_);

    policy_.clear();
    policy_.loadPolicyFile(fileName);
  }
  
  void 
  BehaviourEngineImpl::openPolicy()
    ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy))
  {
    Guard guard(mutex_);
    policy_.open();
  }
  
  void
  BehaviourEngineImpl::closePolicy ()
    ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy))
  {
    Guard guard(mutex_);

    if (!policy_.valid())
      throw Miro::BehaviourEngine::ENoPolicy();
    policy_.close();
  }  
};
