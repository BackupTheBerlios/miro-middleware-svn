// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ActionPattern.h"

#include "BehaviourEngineC.h"

#include "Behaviour.h"
#include "BehaviourParameters.h"
#include "BehaviourRepository.h"
#include "ArbiterRepository.h"
#include "KeyValueList.h"
#include "StructuredPushSupplier.h"

#include <qdom.h>

using std::string;

namespace Miro
{
  Mutex    ActionPattern::transitionMutex_;
  ActionPattern* ActionPattern::currentActionPattern_ = 0;

  ActionPattern::ActionPattern(const string& _name, StructuredPushSupplier * _pSupplier) :
    pSupplier_(_pSupplier),
    arbiter_(NULL),
    arbiterParameters_(NULL),
    actionPatternName_(_name)
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


  void
  ActionPattern::xmlInit(const QDomNode& _node, const ActionPatternMap& _apMap) 
  {
    ArbiterRepository * ar = ArbiterRepository::instance();
    BehaviourRepository * br = BehaviourRepository::instance();

    // first pass
    // retrieve arbiter

    // we need it to register the behaviours there

    QDomNode n = _node.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) {            // the node was really an element.

	QDomAttr attribute = e.attributeNode("name");

	// retrieve arbiter
	if (e.tagName()=="arbiter") {
	  if (arbiter_ == NULL) {
	    if (!attribute.isNull() && !attribute.value().isEmpty()) {
	      string name(attribute.value());
	      Arbiter* a;
	      if ((a = ar->getArbiter(name)) != 0) {
		ArbiterParameters * params = a->getParametersInstance();
		arbiter(a, params);
		
	      } 
	      else {
		std::string error("Arbiter not registered: " + name);
		throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
	      }
	    } 
	    else {
	      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Arbiter without name."));
	    }
	  }
	  else {
	   throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Multiple arbiters specified."));
	  }
	}
	// syntax checking 
	else if (e.tagName() != "behaviour" && e.tagName() != "transition") {
	  std::string error("Unknown tag name: " + string(e.tagName()));
	  throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
	}
      }
      n = n.nextSibling();
    }

    // There has to be exact one arbiter
    if (arbiter_ == NULL) {
      std::string error("ActionPattern without an arbiter: " + actionPatternName_);
      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
    }
    
    // second pass 
    // retrieve behaviours and transitions
    
    n = _node.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) {            // the node was really an element.

	QDomAttr attribute = e.attributeNode("name");

	// retrieve behaviours
	if (e.tagName() == "behaviour") {
	  if (!attribute.isNull() && !attribute.value().isEmpty()) {
	    string name(attribute.value());

	    Behaviour * behaviour;
	    BehaviourParameters * parameters;
	    if ((behaviour = br->getBehaviour(name)) != 0) {
	      parameters = behaviour->getParametersInstance();

	      KeyValueList params;
	      params <<= n;

	      *parameters <<= params;
	      addBehaviour(behaviour, parameters);
	    } 
	    else {
	      std::string error("Behaviour not registered: " + name);
	      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
	    }
	  } 
	  else {
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Behaviour without name."));		
	  }
	}

	// retrieve transitions
	else if (e.tagName() == "transition") {
	  QDomAttr attrMessage = e.attributeNode("message");
	  string message;

	  if (!attrMessage.isNull() && !attrMessage.value().isEmpty()) {
	    message = string(attrMessage.value());
	  } 
	  else {
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Transition without message."));
	  }
	  
	  QDomAttr attrPattern = e.attributeNode("target");
	  if (!attrPattern.isNull() && !attrPattern.value().isEmpty()) {
	    string patternname = string(attrPattern.value());
	    ActionPatternMap::const_iterator iter = _apMap.find(patternname);

	    if (iter != _apMap.end())
	      addTransition(message, iter->second);
	    else {
	      std::string error("ActionPattern for transition not registered: " + 
				message + " --> " + patternname + ".");
	      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
	    }
	  }
	  else
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Transition without target."));
	}
      }
      n = n.nextSibling();
    }
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
    ACE_Time_Value t(0, 100000);
    t += ACE_OS::gettimeofday();

    if (transitionMutex_.acquire(t) == -1) {
      std::cerr << "Transition mutex blocked!" << endl;
      return;
    }
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
    transitionMutex_.release();
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
    if (currentActionPattern_) {
      currentActionPattern_->close(NULL);
      currentActionPattern_ = NULL;
    }
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
