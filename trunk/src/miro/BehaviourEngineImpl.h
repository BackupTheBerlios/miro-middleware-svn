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
#ifndef BehaviourEngineImpl_h
#define BehaviourEngineImpl_h

#include "BehaviourEngineS.h"
#include "Policy.h"
#include "Synch.h"

namespace Miro
{
  // forward declarations
  class StructuredPushSupplier;
  class Policy;

  class BehaviourEngineImpl : public virtual POA_Miro::BehaviourEngine
  {
  public:
    //Constructor 
    BehaviourEngineImpl(StructuredPushSupplier * _pSupplier = NULL);
  
    //Destructor 
    virtual ~BehaviourEngineImpl();
  
    virtual void loadPolicy (const char * policy)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EMalformedXML,
		       Miro::BehaviourEngine::EMalformedPolicy));

    virtual void loadPolicyFile (const char * fileName)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EMalformedXML,
		       Miro::BehaviourEngine::EMalformedPolicy,
		       Miro::BehaviourEngine::EFile));

    virtual void openPolicy ()
      ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy));

    virtual void closePolicy ()
      ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy));

  protected:
    Mutex mutex_;
    Policy policy_;
  };
};

#endif // BehaviourEngineImpl_h
