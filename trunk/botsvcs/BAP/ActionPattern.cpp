// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ActionPattern.h"
#include "Policy.h"

#include "idl/BehaviourEngineC.h"

#include "Behaviour.h"
#include "BehaviourParameters.h"
#include "BehaviourRepository.h"
#include "ArbiterRepository.h"

#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

#include <qdom.h>

namespace Miro
{
  namespace BAP
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
      Super(NULL, _name),
      pSupplier_(_pSupplier),
      arbiter_(NULL),
      arbiterParameters_(NULL)
    {
      if (pSupplier_) {
	std::cout << "Prepare pattern change event" << std::endl;

	// Status Notify Event initialization
	notifyEvent.header.fixed_header.event_type.domain_name = 
	  CORBA::string_dup(pSupplier_->domainName().c_str());
	notifyEvent.header.fixed_header.event_type.type_name = 
	  CORBA::string_dup("PatternChange");
	notifyEvent.header.fixed_header.event_name = 
	  CORBA::string_dup(getName().c_str());
	notifyEvent.header.variable_header.length(0);   // put nothing here
	notifyEvent.filterable_data.length(0);          // put nothing here
      }
    }

    ActionPattern::ActionPattern(const ActionPattern& _rhs) :
      Super(_rhs),
      pSupplier_(_rhs.pSupplier_),
      arbiter_(_rhs.arbiter_),
      arbiterParameters_(arbiter_->getParametersInstance(*_rhs.arbiterParameters_)),
      notifyEvent(_rhs.notifyEvent)
    {
      BehaviourMap::const_iterator first, last = _rhs.behaviourMap_.end();
      for (first = _rhs.behaviourMap_.begin(); first != last; ++first) {
	addBehaviour(first->second.first, 
		     first->second.first->getParametersInstance(*first->second.second));
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
    
    ActionPattern *
    ActionPattern::clone() const 
    {
      return new ActionPattern(*this);
    }
    
    /**
     * Init the action pattern from the XML description of the policy.
     *
     * @param _node Node within the XML-Tree containing the behaviour.
     * @param _apMap Map of the other existing action patterns of the policy,
     * for transition links.
     */
    void
    ActionPattern::xmlInit(const QDomElement& _node) 
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

		try {
		  Arbiter * a = ar->get(name);
		  ArbiterParameters * params = a->getParametersInstance();
		  arbiter(a, params);
		} 
		catch (ArbiterRepository::ENotRegistered const& e) {
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
	std::string error("ActionPattern without an arbiter: " + getName());
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
	      
	      try {
		Behaviour * behaviour = br->get(name);
		BehaviourParameters * parameters =
		  behaviour->getParametersInstance();
		
		*parameters <<= n;
		addBehaviour(behaviour, parameters);
	      } 
	      catch (ArbiterRepository::ENotRegistered const& e) {
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
	    xmlAddTransition(e);
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
    ActionPattern::open(Pattern * const _predecessor,
			std::string const& _transition) 
    {
      MIRO_LOG_OSTR(LL_NOTICE, "New Pattern: " << getName());

      if (_predecessor != NULL) {
	// disable behaviours which are not needed anymore
	_predecessor->close(this);
      }
      
      MIRO_LOG(LL_NOTICE, "Init arbiter.");
      arbiter_->init(arbiterParameters_);
      
      parent_->currentPattern(this);

      if (!(arbiter_->isActive())) {
	MIRO_LOG_OSTR(LL_NOTICE, "Activate arbiter: " << arbiter_->getName());
	arbiter_->open();
      }
      
      //enable all current behaviours		
      MIRO_LOG_OSTR(LL_NOTICE, "Enabling behaviours.");
      BehaviourMap::const_iterator i;
      for(i = behaviourMap_.begin(); i != behaviourMap_.end(); i++) {
	i->second.first->init(i->second.second);
	// if a behaviour is active, check for disabling
	if (!(i->second.first->isActive())) {
	  MIRO_LOG_OSTR(LL_NOTICE, "Enabling Behaviour: " << i->second.first->getBehaviourName());
	  i->second.first->open();			
	}
      }
      
      // send pattern change debug output
      if (pSupplier_) {
	std::cout << "send pattern change" << std::endl;
	notifyEvent.header.fixed_header.event_name = 
	  CORBA::string_dup(getFullName().c_str());
	notifyEvent.remainder_of_body <<= CORBA::string_dup(_transition.c_str());
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
    ActionPattern::close(ActionPattern * const _successor)
    {
      MIRO_LOG(LL_NOTICE, "Disabling behaviours.");
      BehaviourMap::const_iterator i;
      // disable behaviours which are not needed anymore
      for(i = behaviourMap().begin(); i != behaviourMap().end(); ++i) {
	// if a behaviour is active, check for disabling
	if (i->second.first->isActive()) {
	  // if the next actionpattern doesn't need this behaviour...
	  if (_successor == NULL || 
	      _successor->getBehaviour(i->first) == NULL) {
	    //disable behaviour
	    MIRO_LOG_OSTR(LL_NOTICE, "Disabling Behaviour: " << i->second.first->getBehaviourName());
	    i->second.first->close();			
	  }
	}
      }
      if (_successor == NULL || 
	  _successor->arbiter() != arbiter_) {
	MIRO_LOG_OSTR(LL_NOTICE, "Disabling arbiter: " << arbiter_->getName());
	arbiter_->close();
      }
      
      MIRO_LOG(LL_NOTICE, "Disabling done.");
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
    ActionPattern::getBehaviourParameters(std::string const& _pattern,
					  std::string const& _behaviour) const
    {
      if (_pattern == "") {
	BehaviourMap::const_iterator b = behaviourMap_.find(_behaviour);
	if (b != behaviourMap_.end()) {
	  Guard guard(transitionMutex());
	  return b->second.first->getParametersInstance(*(b->second.second));
	}
      }
      
      throw BehaviourEngine::EUnknownActionPattern();
    }
    
    void
    ActionPattern::setBehaviourParameters(std::string const& _pattern,
					  std::string const& _behaviour, 
					  BehaviourParameters * _parameters) 
    {
      if (_pattern == "") {
	_parameters->pattern = this;
	
	BehaviourMap::iterator b = behaviourMap_.find(_behaviour);
	if (b != behaviourMap_.end()) {
	  Guard guard(transitionMutex());
	  BehaviourParameters * p = b->second.second;
	  b->second.second = _parameters;
	  
	  // the action pattern is currently running
	  if (currentPattern() == this) {
	    // delegate deletion of parameters to the behaviour
	    b->second.first->deleteParametersOnTransition();
	    // init the behaviour with new parameters
	    b->second.first->init(b->second.second);
	  }
	  else
	    delete p;

	  return;
	}
      }

      throw BehaviourEngine::EUnknownActionPattern();
    }
    
    Behaviour * 
    ActionPattern::getBehaviour(const std::string& _behaviourName)
    {
      BehaviourMap::const_iterator i = behaviourMap_.find(_behaviourName);
      return (i != behaviourMap_.end())? i->second.first : NULL;
    }
    
    void 
    ActionPattern::sendTransitionMessage(const std::string& _message) 
    {
      MIRO_ASSERT(parent_ != NULL);
      
      // timeout is a tenth of a second
      ACE_Time_Value t(0, 100000);
      t += ACE_OS::gettimeofday();
      
      if (transitionMutex().acquire(t) != -1) {
	protectedSendTransitionMessage(_message);
      }
      else {
	// this can happen due to Reactor/Timed-Behaviour upcalls.
	MIRO_LOG(LL_WARNING, "Transition mutex blocked!");
      }
    }
    
    void 
    ActionPattern::protectedSendTransitionMessage(const std::string& _message) 
    {
      // make sure this is still the current action pattern
      // if the policy is not active anymore, its current pattern will be NULL
      if (currentPattern() == this) {
	MIRO_DBG_OSTR(MIRO, LL_DEBUG, 
		      "ActionPattern: Transition " << _message);
	Super::sendTransitionMessage(_message);
      }
      else {
	if (parent_->currentPattern()) {
	  MIRO_LOG_OSTR(LL_NOTICE, 
			"Asynch transition. Current pattern: " << 
			parent_->currentPattern()->getName());
	}
	else {
	  MIRO_LOG_OSTR(LL_ERROR, 
			"No current pattern for action pattern: " <<
			getName());
	}
      }
      transitionMutex().release();
    }
    
    void 
    ActionPattern::printToStream(std::ostream& ostr) const
    {
      ostr << Indent::ws() << "ActionPattern: " << getName() << std::endl;
      Indent indent;
      Super::printToStream(ostr);
      
      BehaviourMap::const_iterator j;
      for(j = behaviourMap_.begin(); j != behaviourMap_.end(); ++j) {
	ostr << Indent::ws() << "Behaviour: " << j->first << std::endl
	     << Indent::ws() << "Parameters: " << std::endl;

	Indent indent;
	std::stringstream sstr;
	sstr << *(j->second.second);

	std::string line;
	while (getline(sstr, line)) {
	  ostr << Indent::ws() << line << std::endl;
	}
      }
      ostr << Indent::ws() << "Arbiter: " << arbiter_->getName() << std::endl
	   << Indent::ws() << "Parameters: " << std::endl;

      Indent indent2;
      std::stringstream sstr;
      sstr << *arbiterParameters_ << std::endl;

      std::string line;
      while (getline(sstr, line)) {
	ostr << Indent::ws() << line << std::endl;
      }
    }

    ActionPattern *
    ActionPattern::currentLeafPattern()
    {
      return this;
    }
  }
}
