// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Behaviour_h
#define Behaviour_h

#include "BehaviourParameters.h"
#include "VelocitySpace.h"
#include "ActionPattern.h"
#include "Arbiter.h"

#include <string>

#define BEHAVIOUR_PARAMETERS_FACTORY(X) \
    virtual X * getParametersInstance() const; \
    virtual X * getParametersInstance(const ::Miro::BehaviourParameters& _p) const

#define BEHAVIOUR_PARAMETERS_FACTORY_IMPL(C, X) \
  X * \
  C::getParametersInstance() const \
  { \
    return new X(); \
  } \
  X * \
  C::getParametersInstance(const ::Miro::BehaviourParameters& _p) const \
  { \
    const X& p= dynamic_cast<const X&>(_p); \
    return new X(p); \
  }

namespace Miro
{
  // forward declarations
  class Parameter;
  class BehaviourParameters;
  class ArbiterMessage;

  //! Base class of all behaviours.
  /**
   * @author Ingmar Baetge
   * @author Hand Utz
   */
  class Behaviour 
  {
  public:
    //! Default constructor.
    Behaviour();
    //! Virtual destructor.
    virtual ~Behaviour();

    //! Factory method for BehaviourParameters.
    virtual BehaviourParameters * getParametersInstance() const;
    //! Factory method for BehaviourParameters.
    virtual BehaviourParameters * getParametersInstance(const BehaviourParameters& _p) const;

    //! Bring the behaviour in active state.
    virtual void open();
    //! Terminate the active state of the behaviour.
    virtual void close();
    //! Initialize the behaviour with a new parameter set.
    virtual void init(const BehaviourParameters * _params);

    //! Callback for the actual implementation of the behaviour.
    virtual void action();
    //! Callback for the default implementation of the action callback.
    virtual std::string actionTake2(BehaviourParameters const * _params,
				    ArbiterMessage * _message);

    //! Return the name of the behaviour.
    /**
     * For a policy to be constructed correctly all behaviours within 
     * the behaviour repository have to report a unigue name by this method.
     */
    virtual const std::string& getBehaviourName() const = 0;

    //! Returns true if the behaviour is in active state.
    bool isActive();

    //! Hook for the ConstraintArbiter
    /** @todo replace by a method pointer within the arbitration message. */
    virtual void addEvaluation(VelocitySpace *);

    //! Initiate a memory cleanup during dynamic policy reconfiguration.
    void deleteParametersOnTransition();
  protected:
    //! Send a transition message to the policy.
    void sendMessage(const std::string& message);
    //! Send an arbitration message to the Arbiter of the ActionPattern.
    void arbitrate(const ArbiterMessage& message);
    
    //! Get the parameter set of an existing behaviour within an actionpattern in the transition set.
    BehaviourParameters * const 
    getSuccessorBehaviourParameters(const std::string& _transition,
				    const std::string& _behaviour) const;
    //! Set the parameters of an existing behaviour within an actionpattern in the transition set.
    void 
    setSuccessorBehaviourParameters(const std::string& _transition,
				    const std::string& _behaviour, 
				    BehaviourParameters * _parameters);

    //! Flag indicating the behaviour to be active.
    /** Default is false. */
    bool active_;
    //! Parameter set of the behaviour.
    /** 
     * This is exchanged on a transition to another action pattern.
     * Default is the NULL-Pointer. 
     */
    const BehaviourParameters * params_;
    //! Hold the old parameters for cleanup on dynamic policy reconfiguartion.
    /** Default is NULL. */
    const BehaviourParameters * oldParams_;
  };

  inline
  bool 
  Behaviour::isActive() { 
    return active_; 
  };

  inline
  void
  Behaviour::deleteParametersOnTransition() {
    oldParams_ = params_;
  }

  inline
  void
  Behaviour::arbitrate(const ArbiterMessage& message) {
    params_->pattern->arbiter()->arbitrate(message);
  }
}
#endif
