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


#include "ActionPattern.h"

#include "Behaviour.h"
#include "BehaviourParameters.h"

#include "StructuredPushSupplier.h"

using std::string;

namespace Miro
{
  Mutex    ActionPattern::transitionMutex_;
  ActionPattern* ActionPattern::currentActionPattern_ = 0;

  ActionPattern::ActionPattern(const string &name, StructuredPushSupplier * _pSupplier) :
    pSupplier_(_pSupplier),
    actionPatternName_(name)
  {
    if (pSupplier_) {
      // Status Notify Event initialization
      notifyEvent.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(pSupplier_->domainName().c_str());
      notifyEvent.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("PatternChange");
      notifyEvent.header.fixed_header.event_name = 
	CORBA::string_dup(actionPatternName_.c_str());
      notifyEvent.header.variable_header.length(0);   // put nothing here
      notifyEvent.filterable_data.length(0);          // put nothing here
    }
  }

  ActionPattern::~ActionPattern()
  {
  }

  void ActionPattern::open() 
  {
    cout << "ActionPattern: New Pattern: " << getActionPatternName() << endl;

    if (ActionPattern::currentActionPattern() != 0) {
      // disable behaviours which are not needed anymore
      ActionPattern::currentActionPattern()->close(this);
    }

    cout << "ActionPattern: Init arbiter." << endl;
    arbiter_->init(arbiterParameters_);

    setCurrentActionPattern(this);
    
    if (!(arbiter_->isActive())) {
      cout << "ActionPattern: Activate arbiter: " << arbiter_->getName() << endl;
      arbiter_->open();
    }
	
    //enable all current behaviours		
    cout <<"ActionPattern: Enabling behaviours." << endl;
    BehaviourMap::const_iterator i;
    for(i = behaviourMap_.begin(); i != behaviourMap_.end(); i++) {
      i->second.first->init(i->second.second);
      // if a behaviour is active, check for disabling
      if (!(i->second.first->isActive())) {
	cout << "Enabling Behaviour: " << i->second.first->getBehaviourName() << endl;
	i->second.first->open();			
      }
    }

    // send pattern change debug output
    if (pSupplier_) {
      pSupplier_->sendEvent(notifyEvent);
    }
  }

  void
  ActionPattern::close(ActionPattern * nextPattern)
  {
    cout << "Disabling behaviours." << endl;
    BehaviourMap::const_iterator i;
    // disable behaviours which are not needed anymore
    for(i = behaviourMap().begin(); i != behaviourMap().end(); ++i) {
      // if a behaviour is active, check for disabling
      if (i->second.first->isActive()) {
	// if the next actionpattern doesn't need this behaviour...
	if (nextPattern == NULL || nextPattern->getBehaviour(i->first) == 0) {
	  //disable behaviour
	  cout << "Disabling Behaviour: " << i->second.first->getBehaviourName() << endl;
	  i->second.first->close();			
	}
      }
    }
    if (nextPattern == 0 || nextPattern->arbiter() != arbiter_) {
      cout << "Disabling arbiter: " << arbiter_->getName() << endl;
      arbiter_->close();
    }

    cout << "Disabling done." << endl;
  }

  const string&
  ActionPattern::getActionPatternName() const {
    return actionPatternName_;
  }

  void
  ActionPattern::addBehaviour(Behaviour * _behaviour, BehaviourParameters * _parameters) 
  {
     behaviourMap_[_behaviour->getBehaviourName()] = std::make_pair(_behaviour, _parameters);	
    arbiterParameters_->registerBehaviour(_behaviour);
  }

  void 
  ActionPattern::addTransition(const string& _patternName, ActionPattern* _actionPattern) 
  {
    transitionTable_[_patternName] = _actionPattern;	
  }

  Behaviour* 
  ActionPattern::getBehaviour(const string& _behaviourName) {
    BehaviourMap::const_iterator i = behaviourMap_.find(_behaviourName);
    return (i != behaviourMap_.end())? i->second.first : NULL;
  }

  ActionPattern* 
  ActionPattern::getTransitionPattern(const string& _patternName) {
    TransitionMap::const_iterator i = transitionTable_.find(_patternName);
    return (i != transitionTable_.end())? i->second : NULL;
  }

  void 
  ActionPattern::sendMessage(Behaviour const * const, const std::string& message) 
  {
    Miro::Guard guard(transitionMutex_);
    // make sure this is still the current action pattern
    if (currentActionPattern_ == this) {
      cout << "ActionPattern: Transition " << message << endl;

      ActionPattern* ap = getTransitionPattern(message);
      if (ap != currentActionPattern_) { 
	if (ap != NULL)
	  ap->open();
	else {
	  cerr << "ActionPattern: no transition registered for message! \a" 
	       << endl;
	}
      }
    }
  }

  void 
  ActionPattern::printToStream(ostream& ostr) const
  {
    ostr << "ActionPattern: " << getActionPatternName() << endl;
 		
    ostr << "Transitions:" << endl;
    TransitionMap::const_iterator i;
    for(i = transitionTable_.begin(); i != transitionTable_.end(); ++i) {
      ostr << "Transition: " << i->first << "-->"
	   << ((i->second)? i->second->getActionPatternName() : "NULL")
	   << endl;
    }

    BehaviourMap::const_iterator j;
    for(j = behaviourMap_.begin(); j != behaviourMap_.end(); ++j) {
      ostr << "Behaviour: " << j->first << "  "
	   << j->second.first->getBehaviourName() << endl
	   << "Parameters: " << endl
	   << *(j->second.second)
	   << endl;
    }
  }

  void
  ActionPattern::closeCurrentActionPattern()
  {
    Miro::Guard guard(transitionMutex_);
    currentActionPattern_->close(NULL);
    currentActionPattern_->arbiter_->close();
  }

  void 
  ActionPattern::sendTransitionMessage(const std::string& message) 
  {
    ActionPattern * pattern = currentActionPattern();
    if (pattern)
      pattern->sendMessage(NULL, message);
  }
  


  std::ostream&
  operator << (std::ostream& ostr, const ActionPattern& _pattern)
  {
    _pattern.printToStream(ostr);

    return ostr;
  }
};
