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

  //! Implementation of the BehaviourEngine CORBA interface.
  class BehaviourEngineImpl : public virtual POA_Miro::BehaviourEngine
  {
  public:
    //! Constructor .
    BehaviourEngineImpl(StructuredPushSupplier * _pSupplier = NULL,
			bool _openOnLoad = false);
  
    //! Virtual destructor.
    virtual ~BehaviourEngineImpl();

    //! Query wheter the behaviour engine will automatically open a loaded poliy.
    bool openOnLoad() const;
    //! Set wheter the behaviour engine is to start automatically an policy after loading.
    void openOnLoad(bool flag);
    //! Get const reference to the policy.
    const Policy& policy() const;
    //! Get reference to the policy.
    Policy& policy();

  
    //! BehaviourEngine interface method implementation.
    virtual void loadPolicy (const char * policy)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EMalformedXML,
		       Miro::BehaviourEngine::EMalformedPolicy));
    //! BehaviourEngine interface method implementation.
    virtual void loadPolicyFile (const char * fileName)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EMalformedXML,
		       Miro::BehaviourEngine::EMalformedPolicy,
		       Miro::BehaviourEngine::EFile));
    //! BehaviourEngine interface method implementation.
    virtual void openPolicy ()
      ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy));
    //! BehaviourEngine interface method implementation.
    virtual void closePolicy ()
      ACE_THROW_SPEC ((Miro::BehaviourEngine::ENoPolicy));
    //! BehaviourEngine interface method implementation.
    virtual void openActionPattern(const char * pattern)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EUnknownActionPattern));
    //! BehaviourEngine interface method implementation.
    virtual void sendTransition(const char * transition) ACE_THROW_SPEC (());
    //! BehaviourEngine interface method implementation.
    virtual void sendTransitionFromActionPattern(const char * pattern,
						 const char * transition)
      ACE_THROW_SPEC ((Miro::BehaviourEngine::EUnknownActionPattern,
		       Miro::BehaviourEngine::EUnregisteredTransition));

  protected:
    //! Mutex to synchronise concurrent access.
    Mutex mutex_;
    //! The policy object to controll.
    Policy policy_;

    //! Flag indicating whether to open a policy automatically after loading or not.
    /** Default is false. */
    bool openOnLoad_;
  };

  inline
  bool
  BehaviourEngineImpl::openOnLoad() const {
    return openOnLoad_;
  }
  inline
  void
  BehaviourEngineImpl::openOnLoad(bool flag) {
    openOnLoad_ = flag;
  }
  inline
  const Policy&
  BehaviourEngineImpl::policy() const {
    return policy_;
  }
  inline
  Policy&
  BehaviourEngineImpl::policy() {
    return policy_;
  }

};

#endif // BehaviourEngineImpl_h
