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
#ifndef ActionPattern_h
#define ActionPattern_h

#include "Synch.h"
#include "Policy.h"
#include "ArbiterParameters.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <map>
#include <string>
#include <iostream>

namespace Miro
{

  // forward declarations
  class Policy;
  class KeyValueList;
  class Behaviour;
  class BehaviourParameters;
  class Arbiter;
  class ActionPattern;
  class StructuredPushSupplier;

  //! Ostream operator for debug purposes.
  std::ostream& operator << (std::ostream& ostr, const ActionPattern&);

  //! Class representing an ation pattern.
  /** 
   * This class represents an action pattern, which consists of a set of 
   * behaviours, an arbiters and their parameters
   * @author Ingmar Baetge
   * @author Hans Utz
   */
  class ActionPattern 
  {
  public:
    //! A pair of a behaviour and its parameter set.
    typedef std::pair<Behaviour *, BehaviourParameters *> BehaviourPair;
    //! A map matching behaviours by name.
    typedef std::map<std::string, BehaviourPair> BehaviourMap;
    //! A map matching transitions to the following ActionPattern.
    typedef std::map<std::string, ActionPattern*> TransitionMap;
    //! A map of existing action pattern to link transitions to.
    /** @todo Should go back into Policy,along with the second parsing run. */
    typedef std::map<std::string, ActionPattern*> ActionPatternMap;

    //! Initializing constructor.
    ActionPattern(const std::string& _name, 
		  StructuredPushSupplier * _pSupplier = NULL);
    //! Virtual destructor.
    virtual ~ActionPattern();

    //! Initialize the action pattern from an XML description.
    void xmlInit(const QDomNode& _node, const ActionPatternMap& apMap);
    //! Query the name of the action pattern.
    const std::string& getActionPatternName() const;
    //! Add a behaviour and its parameter set to the action pattern.
    void addBehaviour(Behaviour * _behaviour, 
		      BehaviourParameters * _parameters);
    //! Get the parameters of an existing behaviour.
    BehaviourParameters * const getBehaviourParameters(const std::string& _behaviour) const;
    //! Set the parameters of an existing behaviour.
    void setBehaviourParameters(const std::string& _behaviour,
				BehaviourParameters * _parameters);
    //! Add a transition to another action pattern.
    void addTransition(const std::string&, ActionPattern *);
    //! Set the arbiter of the action pattern.
    void arbiter(Arbiter * _a, ArbiterParameters * _p);
    //! Lookup a behaviour of the action pattern by name.
    Behaviour * getBehaviour(const std::string &);
  
    //! Send a transition message, switching to another action pattern.
    void sendMessage(Behaviour const * const sender, 
		     const std::string& message);

    ActionPattern * getTransitionPattern(const std::string &);

    //! Send a transition message, switching to another action pattern.
    void sendTransitionMessage(const std::string& message);

    //! Get the arbiter.
    Arbiter * arbiter();
   protected:
    //! Retrieve the current action pattern.
    ActionPattern * const currentActionPattern();

    //! Activate the action pattern.
    void open();
    //! Deactivate the action pattern.
    void close(ActionPattern * nextPattern);
    //! Dump action pattern configuration to ostream for debugging.
    virtual void printToStream(std::ostream& ostr) const;

    const BehaviourMap& behaviourMap() const;
   // class data

    //! Policy the action pattern belongs to.
    Policy * policy_;
    //! Event supplier for online output.
    /** Default is the NULL pointer. */
    StructuredPushSupplier * pSupplier_;
    //! The arbiter of the action pattern.
    /** Initialized to NULL */
    Arbiter * arbiter_;
    //! The parameters of the arbiter of the action pattern.
    /** Initialized to NULL */
    ArbiterParameters * arbiterParameters_;

    //! Map of the behaviours and their parameters.
    BehaviourMap behaviourMap_;
    //! Map of the transitions and the follow up action patterns.
    TransitionMap transitionTable_;
    //! The name of the action pattern.
    std::string actionPatternName_;
    //! Preconfigured event to send to the notification service.
    CosNotification::StructuredEvent notifyEvent;

    friend std::ostream& operator << (std::ostream& ostr, const ActionPattern&);

    friend class Policy;
  };

  inline
  const std::string&
  ActionPattern::getActionPatternName() const {
    return actionPatternName_;
  }

  inline 
  void
  ActionPattern::arbiter(Arbiter * _a, ArbiterParameters * _p) {
    _p->pattern = this;

    arbiter_ = _a;
    arbiterParameters_ = _p;
  }

  inline 
  Arbiter *
  ActionPattern::arbiter() {
    return arbiter_;
  }
  inline
  const ActionPattern::BehaviourMap&
  ActionPattern::behaviourMap() const {
    return behaviourMap_;	
  }

  inline
  ActionPattern * const
  ActionPattern::currentActionPattern() {
    return policy_->currentActionPattern();
  }
};
#endif
