// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ActionPattern_h
#define ActionPattern_h

#include "Synch.h"
#include "ArbiterParameters.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <map>
#include <string>
#include <iostream>

// forward declarations

namespace Miro
{

  // forward declarations
  class KeyValueList;
  class Behaviour;
  class BehaviourParameters;
  class Arbiter;
  class ActionPattern;
  class StructuredPushSupplier;

  std::ostream& operator << (std::ostream& ostr, const ActionPattern&);

  /** This class represents an action pattern, which consists of a set of 
   * behaviours, arbiters and their configuration
   * @author Ingmar Baetge
   */
  class ActionPattern 
  {
  public:
    typedef std::pair<Behaviour *, BehaviourParameters *> BehaviourPair;
    typedef std::map<std::string, BehaviourPair> BehaviourMap;
    typedef std::map<std::string, ActionPattern*> TransitionMap;
    typedef std::map<std::string, ActionPattern*> ActionPatternMap;

    ActionPattern(const std::string& _name, StructuredPushSupplier * _pSupplier = NULL);
    virtual ~ActionPattern();

    void xmlInit(const QDomNode& _node, const ActionPatternMap& apMap);

    void open();
    void close(ActionPattern * nextPattern);

    const std::string& getActionPatternName() const;

    void addBehaviour(Behaviour * _behaviour, BehaviourParameters * _parameters);
    void addTransition(const std::string&, ActionPattern *);

    void arbiter(Arbiter * _a, ArbiterParameters * _p);
    Arbiter * arbiter();
  
    Behaviour * getBehaviour(const std::string &);	
    const BehaviourMap& behaviourMap() const;
  
    void sendMessage(Behaviour const * const sender, const std::string& message);

    // static methods

    static void sendTransitionMessage(const std::string& message);
    static ActionPattern * currentActionPattern();
    static void closeCurrentActionPattern();

  protected:
    ActionPattern * getTransitionPattern(const std::string &);

    virtual void printToStream(std::ostream& ostr) const;


    // class data

    StructuredPushSupplier * pSupplier_;
    Arbiter * arbiter_;
    ArbiterParameters * arbiterParameters_;

    BehaviourMap behaviourMap_;
    TransitionMap transitionTable_;

    std::string actionPatternName_;
    CosNotification::StructuredEvent notifyEvent;

    // protected static methods

    static void setCurrentActionPattern(ActionPattern *);

    // static data

    static Mutex transitionMutex_;
    static ActionPattern* currentActionPattern_;

    friend std::ostream& operator << (std::ostream& ostr, const ActionPattern&);
  };

  inline 
  void
  ActionPattern::arbiter(Arbiter * _a, ArbiterParameters * _p) {
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
  ActionPattern *
  ActionPattern::currentActionPattern() {
    return currentActionPattern_;
  }

  inline
  void 
  ActionPattern::setCurrentActionPattern(ActionPattern* ap) {
    currentActionPattern_ = ap;
  };
};
#endif
