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

#include "ActionPattern.h"

#include "BehaviourEngineC.h"

#include "Behaviour.h"
#include "BehaviourParameters.h"
#include "BehaviourRepository.h"
#include "ArbiterRepository.h"
#include "StructuredPushSupplier.h"

#include <qdom.h>

namespace Miro
{
  /**
   * @param _name The name of the action pattern.
   * @param _pSupplier Pointer to a structured push supplier to send
   * transition events to the notification channel for online
   * supervision of the behaviour engine. If the pointer is NULL, no
   * events will be generated.
   */
  ActionPattern::ActionPattern(const std::string& _name, 
			       StructuredPushSupplier * _pSupplier) :
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
    // Clean up the behaviour parameters
    // to prevent memory leaks.
    BehaviourMap::iterator b, bEnd = behaviourMap_.end();
    for (b = behaviourMap_.begin(); b != bEnd; ++b)
      delete b->second.second;
    delete arbiterParameters_;
  }

  /**
   * Init the action pattern from the XML description of the policy.
   *
   * @param _node Node within the XML-Tree containing the behaviour.
   * @param _apMap Map of the other existing action patterns of the policy,
   * for transition links.
   */
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
	      std::string name(attribute.value().latin1());
	      Arbiter* a;
	      if ((a = ar->getArbiter(name)) != 0) {
		ArbiterParameters * params = a->getParametersInstance();
		arbiter(a, params);
		
	      } 
	      else {
		std::string error("Arbiter not registered: " + name);
		throw BehaviourEngine:: EMalformedPolicy(CORBA::string_dup(error.c_str()));
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
	  std::string error("Unknown tag name: " + 
		            std::string(e.tagName().latin1()));
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
	    std::string name(attribute.value().latin1());

	    Behaviour * behaviour;
	    BehaviourParameters * parameters;
	    if ((behaviour = br->getBehaviour(name)) != NULL) {
	      parameters = behaviour->getParametersInstance();

	      *parameters <<= n;
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
	  std::string message;

	  if (!attrMessage.isNull() && !attrMessage.value().isEmpty()) {
	    message = std::string(attrMessage.value().latin1());
	  } 
	  else {
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Transition without message."));
	  }
	  
	  QDomAttr attrPattern = e.attributeNode("target");
	  if (!attrPattern.isNull() && !attrPattern.value().isEmpty()) {
	    std::string patternname(attrPattern.value().latin1());
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

  /**
   * Activate the action pattern by 
   *
   * - Disabling all behaviours present within the current action
   *   pattern but not in this one.
   * - Init the arbiter of this action pattern.
   * - Set the current action pattern pointer to this.
   * - Activate the arbiter of this action pattern.
   * - Init all behaviours of this action pattern.
   * - Enabling all behaviours not present within the last action
   *   pattern.
   *
   * Send the transition message to the notification channel if available.
   */
  void
  ActionPattern::open() 
  {
    cout << "ActionPattern: New Pattern: " << getActionPatternName() << endl;

    if (currentActionPattern() != NULL) {
      // disable behaviours which are not needed anymore
      currentActionPattern()->close(this);
    }

    cout << "ActionPattern: Init arbiter." << endl;
    arbiter_->init(arbiterParameters_);

    policy_->currentActionPattern(this);
    
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

  /**
   * Deactivate the action pattern by 
   *
   * - Disabling all behaviours present within this action 
   *   pattern but not in the next one.
   * - Disable the arbiter if not present within the next
   *   action pattern.
   *
   * @param nextPattern The action pattern that will be active 
   * next.
   */
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
	if (nextPattern == NULL || 
	    nextPattern->getBehaviour(i->first) == NULL) {
	  //disable behaviour
	  cout << "Disabling Behaviour: " << i->second.first->getBehaviourName() << endl;
	  i->second.first->close();			
	}
      }
    }
    if (nextPattern == NULL || 
	nextPattern->arbiter() != arbiter_) {
      cout << "Disabling arbiter: " << arbiter_->getName() << endl;
      arbiter_->close();
    }

    cout << "Disabling done." << endl;
  }

  /**
   * Adding a behaviour to the action pattern consists of:
   * - initializing the pattern pointer of the  parameters
   *   to this.
   * - insert the behaviour and the parameters into the behaviourMap_.
   * - register the behaviour at the arbiter.
   */
  void
  ActionPattern::addBehaviour(Behaviour * _behaviour,
			      BehaviourParameters * _parameters) 
  {
    _parameters->pattern = this;

    behaviourMap_[_behaviour->getBehaviourName()] = 
      std::make_pair(_behaviour, _parameters);	
    arbiterParameters_->registerBehaviour(_behaviour);
  }

  BehaviourParameters * const
  ActionPattern::getBehaviourParameters(const std::string& _behaviour) const
  {
    BehaviourMap::const_iterator b = behaviourMap_.find(_behaviour);
    if (b != behaviourMap_.end()) {
      Guard guard(policy_->transitionMutex_);
      return b->second.first->getParametersInstance(*(b->second.second));
    }

    throw BehaviourEngine::EUnknownActionPattern();
  }

  void
  ActionPattern::setBehaviourParameters(const std::string& _behaviour, 
					BehaviourParameters * _parameters) 
  {
    _parameters->pattern = this;

    BehaviourMap::iterator b = behaviourMap_.find(_behaviour);
    if (b != behaviourMap_.end()) {
      Guard guard(policy_->transitionMutex_);
      BehaviourParameters * p = b->second.second;
      b->second.second = _parameters;

      // the action pattern is currently running
      if (currentActionPattern() == this) {
	// delegate deletion of parameters to the behaviour
	b->second.first->deleteParametersOnTransition();
	// init the behaviour with new parameters
	b->second.first->init(b->second.second);
      }
      else
	delete p;
    }
    else 
      throw BehaviourEngine::EUnknownActionPattern();
  }

  void 
  ActionPattern::addTransition(const std::string& _patternName, 
			       ActionPattern* _actionPattern) 
  {
    transitionTable_[_patternName] = _actionPattern;	
  }

  Behaviour* 
  ActionPattern::getBehaviour(const std::string& _behaviourName)
  {
    BehaviourMap::const_iterator i = behaviourMap_.find(_behaviourName);
    return (i != behaviourMap_.end())? i->second.first : NULL;
  }

  ActionPattern* 
  ActionPattern::getTransitionPattern(const std::string& _patternName) 
  {
    TransitionMap::const_iterator i = transitionTable_.find(_patternName);
    return (i != transitionTable_.end())? i->second : NULL;
  }

  void 
  ActionPattern::sendMessage(Behaviour const * const, const std::string& message) 
  {
    ACE_Time_Value t(0, 100000);
    t += ACE_OS::gettimeofday();

    if (policy_->transitionMutex_.acquire(t) == -1) {
      // this can happen due to Reactor/Timed-Behaviour upcalls.
      std::cerr << "Transition mutex blocked!" << endl;
      return;
    }
    // make sure this is still the current action pattern
    if (currentActionPattern() == this) {
      cout << "ActionPattern: Transition " << message << endl;

      ActionPattern* ap = getTransitionPattern(message);
      if (ap != currentActionPattern()) { 
	if (ap != NULL)
	  ap->open();
	else {
	  cerr << "ActionPattern: no transition registered for message! \a" 
	       << endl;
	}
      }
    }
    policy_->transitionMutex_.release();
  }

  void 
  ActionPattern::printToStream(std::ostream& ostr) const
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
