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

#include "idl/BehaviourEngineC.h"

#include "Policy.h"
#include "ActionPattern.h"
#include "ArbiterRepository.h"
#include "Arbiter.h"
#include "ArbiterParameters.h"
#include "Behaviour.h"
#include "BehaviourRepository.h"
#include "BehaviourParameters.h"

#include "miro/Repository.h"

#include <qdom.h>
#include <qfile.h>
#include <qstring.h>

namespace Miro
{
  namespace BAP
  {
    typedef Repository<Policy> PolicyRepository;

    /**
     * This crates an unconfigured policy object. That is,
     * @ref valid() will return false.
     *
     * @param _pSupplier Pointer to a structured push supplier to send
     * transition events to the notification channel for online
     * supervision of the behaviour engine. If the pointer is NULL, no
     * events will be generated.
     */
    Policy::Policy(StructuredPushSupplier * _pSupplier) :
      pSupplier_(_pSupplier),
      startPattern_(NULL),
      currentPattern_(NULL),
      valid_(false)
    {
      patternName_ = "Policy";
    }

    /**
     * Loads policy from a file.  
     *
     * @param _parameterFile File name.
     * @param _pSupplier Pointer to a structured push supplier to send
     * transition events to the notification channel for online
     * supervision of the behaviour engine. If the pointer is NULL, no
     * events will be generated.
     */
    Policy::Policy(QDomElement const& _node, 
		   StructuredPushSupplier * _pSupplier) :
      pSupplier_(_pSupplier),
      startPattern_(NULL),
      currentPattern_(NULL),
      valid_(false)
    {
      patternName_ = "Policy";
      xmlInit(_node);
    }

    Policy::Policy(const Policy& _rhs) :
      Super(_rhs),
      pSupplier_(_rhs.pSupplier_),
      startPattern_(NULL),
      currentPattern_(NULL),
      valid_(_rhs.valid_)
    {
      // clone all child patterns
      PatternMap::const_iterator first, last = _rhs.patterns_.end();
      for (first = _rhs.patterns_.begin(); first != last; ++first) {
	Pattern * pattern = first->second->clone();
	if (first->second == _rhs.startPattern_)
	  startPattern_ = pattern;
	registerPattern(pattern);
      }    

      // clone transition table
      for (first = _rhs.patterns_.begin(); first != last; ++first) {
	std::cout << first->first << std::endl;
	PatternMap::iterator i = patterns_.find(first->first);
	MIRO_ASSERT(i != patterns_.end());
	Pattern * pattern = i->second;

	PatternMap::const_iterator f, l = first->second->transitionTable_.end();
	for (f = first->second->transitionTable_.begin(); f != l; ++f) {
	  PatternMap::iterator j = patterns_.find(f->second->getName());
	  MIRO_ASSERT(j != patterns_.end());
	  Pattern * successor = j->second;

	  pattern->addTransition(f->first, successor);
	}
      }    
    }

    Policy::~Policy()
    {
      close();
      PatternMap::iterator first, last = patterns_.end();
      for (first = patterns_.begin(); first != last; ++first)
	delete (first->second);
    }
	

    Policy *
    Policy::clone() const 
    {
      return new Policy(*this);
    }

    /** 
     * @param _filename The name of the file to load.
     */
    void 
    Policy::loadPolicyFile(Policy& _policy, char const* _filename) 
    {
      QDomDocument doc( "MiroPolicyConfigFile" );
      QFile f(_filename);
      if ( !f.open( IO_ReadOnly ) )
	throw BehaviourEngine::EFile();
      if ( !doc.setContent( &f ) ) {
	throw BehaviourEngine::EMalformedXML();
      }

      parsePolicy(_policy, doc);
    }

    /**
     * @param policy The string has to contain a valid XML description
     * of the policy.
     */
    void 
    Policy::loadPolicy(Policy& _policy, char const * _xml) 
    {
      QDomDocument doc( "MiroPolicyConfigFile" );
      QCString p(_xml);
      if (!doc.setContent(p)) {
	throw BehaviourEngine::EMalformedXML();
      }

      parsePolicy(_policy, doc);
    }

    void
    Policy::parsePolicy(Policy& _policy, const QDomDocument& _doc)
    {
      // delete previous policy
      _policy.clear();
      // delete previous subpolicies
      PolicyRepository::instance()->clear();

      QDomNode n = _doc.documentElement();

      // collect all policies
      while( !n.isNull() ) {
	QDomElement e = n.toElement(); // try to convert the node to an element.
	if( !e.isNull() ) {            // the node was really an element.
	  QString tag = e.tagName();
	  if (tag == "policy") {
	    _policy.xmlInit(e);
	  }
	  else if (tag == "structuredpolicy") {
	    bool defaultPolicy = false;

	    QDomNode n2 = e.firstChild();
	    while (!n2.isNull()) {
	      QDomElement e2 = n2.toElement(); // try to convert the node to an element.
	      if (e2.tagName() == "policy") {
		QString start = e2.attribute("start");
		// sub policy
		if (e2.hasAttribute("name") && 
		    start != "true") {
		  Policy * const policy = new Policy(e2, _policy.pSupplier_);
		  PolicyRepository::instance()->add(policy->getName(), policy);
		} 
		else if (!defaultPolicy) {
		  defaultPolicy = true;
		  _policy.xmlInit(e2);
		}
		else {
		  throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Multiple start policies."));
		}
	      }
	      n2 = n2.nextSibling();
	    }
	  }
	  else {
	    QString error = "Non valid main tag: " + e.tagName();
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.latin1()));
	  }
	}
	n = n.nextSibling();
      }                	
    }

    void
    Policy::xmlInit(const QDomElement& _element) 
    {
      PolicyRepository * pr = PolicyRepository::instance();

      clear();

      // set policy name
      if (_element.hasAttribute("name"))
	patternName_ = std::string(_element.attribute("name").latin1());

      // collect all action patterns, subpolicies and transitions
      QDomNode n = _element.firstChild();
      while( !n.isNull() ) {
	QDomElement e = n.toElement(); // try to convert the node to an element.
	if( !e.isNull() ) {            // the node was really an element.

	  Pattern * pattern = NULL;
	  // get name attribute of tag, if available
	  QString patternName = e.attribute("name");

	  //--------------------------------------------------------------------
	  // tag: action pattern
	  if (e.tagName() == "actionpattern") {
	    if (patternName.isEmpty())
	      throw BehaviourEngine::
		EMalformedPolicy(CORBA::string_dup("ActionPattern without a name."));

	    pattern = new ActionPattern(patternName.latin1(), pSupplier_);
	    pattern->parent(this);
	    pattern->xmlInit(e);
	  } 

	  //--------------------------------------------------------------------
	  // tag: subpolicy
	  else if (e.tagName() == "subpolicy") {
	    if (patternName.isEmpty())
	      throw BehaviourEngine::
		EMalformedPolicy(CORBA::string_dup("Subpolicy without a name."));
	      

	    QString typeName = e.attribute("type");
	    if (typeName.isEmpty())
	      throw BehaviourEngine::
		EMalformedPolicy(CORBA::string_dup("Subpolicy without a type."));

	    try {
	      pattern = new Policy(*pr->get(std::string(typeName.latin1())));
	    }
	    catch (PolicyRepository::ENotRegistered const& e) {
	      std::string error = "Subpolicy with unknown type: ";
	      error += e.what();
	      throw BehaviourEngine::
		EMalformedPolicy(CORBA::string_dup(error.c_str() ));
	    }

	    pattern->parent(this);
	    pattern->setName(patternName.latin1());

	    QDomNode m = n.firstChild();
	    while(!m.isNull()) {
	      QDomElement f = m.toElement();
	      if(!f.isNull()) {
		if (f.tagName() == "transition") {
		  pattern->xmlAddTransition(f);
		}
		else {
		  QString error("Illigal tag in subpolicy: " + f.tagName());
		  throw BehaviourEngine::
		    EMalformedPolicy(CORBA::string_dup(error.latin1()));
		}
	      }
	      m = m.nextSibling();
	    }
	  } 

	  //--------------------------------------------------------------------
	  // tag: transition pattern: downward transition
	  else if (e.tagName() == "transitionpattern") {
	    xmlAddTransitionPattern(e);
	  }

	  //--------------------------------------------------------------------
	  // tag: transition
	  else if (e.tagName() == "transition") {
	    xmlAddTransition(e);
	  }

	  // other tag: error
	  else {
	    QString error("Non-actionpattern tag: " + e.tagName());
	    throw BehaviourEngine::
	      EMalformedPolicy(CORBA::string_dup(error.latin1()));
	  }


	  if (pattern != NULL) {
	    registerPattern(pattern);
	    if (e.attribute("start") == "true"){
	      startPattern_ = pattern;
	    }
	  }
	}
	n = n.nextSibling();
      }                	

      if (startPattern_ == NULL)
	throw BehaviourEngine::
	  EMalformedPolicy(CORBA::string_dup("No start pattern specified."));
  
      // Init childs transition tables
      lowerTransitions();

      // If we could parse it, then it's a good policy   
      valid_ = true;
    }

    void 
    Policy::xmlAddTransitionPattern(QDomElement _element)
    {
      // a transition must have a message tag
      QDomAttr attrMessage = _element.attributeNode("message");
      if (attrMessage.isNull() || 
	  attrMessage.value().isEmpty()) {
	throw BehaviourEngine::
	  EMalformedPolicy(CORBA::string_dup("Transition pattern without message."));
      }
      std::string message = attrMessage.value().latin1();
    
      // a transition must have a target tag
      QDomAttr attrPattern = _element.attributeNode("target");
      if (attrPattern.isNull() ||
	  attrPattern.value().isEmpty()) {
	throw BehaviourEngine::
	  EMalformedPolicy(CORBA::string_dup("Transition pattern without target."));
      }
      std::string target = attrPattern.value().latin1();

      transitionPatterns_.insert(std::make_pair(message, target));
    }



    /**
     * This will activate the actionpattern indicated as
     * start pattern within the policy configuration.
     * 
     */
    void
    Policy::open(Pattern * const _predecessor, std::string const& _transition)
    {
      if (!valid())
	throw BehaviourEngine::ENoPolicy();

      if (startPattern_) {
	startPattern_->open(_predecessor, _transition);
	currentPattern_ = startPattern_;
      }
      else
	throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("No start pattern specified."));
    }
  
    /**
     * This will activate an actionpattern within the policy,
     * deactivating any previously active pattern (if any).
     */
    void 
    Policy::openPattern(const std::string& _pattern)
    {
      std::string first, rest;
      splitPath(_pattern, first, rest);
      Pattern * pattern = getPattern(first);
      if (pattern) {
	if (rest != "") {
	  Policy * policy = dynamic_cast<Policy *>(pattern);
	  if (policy == NULL)
	    throw BehaviourEngine::EUnknownActionPattern();
	  policy->openPattern(rest);
	}
	else {
	  pattern->open(NULL, std::string());
	}
      }
      else
	throw BehaviourEngine::EUnknownActionPattern();
    }

    /**
     * Closes the currently running action pattern.
     * If no pattern is currently running, close is ignored.
     */
    void 
    Policy::close(ActionPattern * const _successor)
    {
      if (currentPattern_ != NULL) {
	currentPattern_->close(_successor);
	currentPattern_ = NULL;
      }
    }

    /**
     * Clears the configuration of the policy.
     * No action pattern will run afterwards, and
     * the object is in unconfigured state. That is,
     * valid() will return false.
     */
    void
    Policy::clear()
    {
      close();
      startPattern_ = NULL;


      PatternMap::iterator first, last = patterns_.end();
      for (first = patterns_.begin(); first != last; ++first)
	delete first->second;
      patterns_.clear();
      transitionPatternTable_.clear();
      transitions_.clear();
      transitionPatterns_.clear();

      valid_ = false;

      Super::clear();
    }

    void
    Policy::registerPattern(Pattern * _pattern) 
    {
      std::cout << "Policy " << getName() << ": Registering " 
		<< _pattern->getName() << std::endl;
      if (!patterns_.insert(std::make_pair(_pattern->getName(),
					   _pattern)).second) {
	std::string error("Action pattern " +
			  _pattern->getName() +
			  "already registered.");
	throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
      }
      _pattern->parent(this);
    }
  
    /**
     * @ param _name The name of the action pattern.
     * @return A pointer to the action pattern or NULL
     * if it does not exist.
     */
    Pattern * const
    Policy::getPattern(const std::string& _name) 
    {
      PatternMap::const_iterator i = patterns_.find(_name);
      if (i != patterns_.end())
	return i->second;
      else
	return NULL;
    }
  
    /**
     * @ param _name The name of the action pattern.
     * @return A pointer to the action pattern or NULL
     * if it does not exist.
     */
    Pattern const * const 
    Policy::getPattern(const std::string& _name) const
    {
      PatternMap::const_iterator i = patterns_.find(_name);
      if (i != patterns_.end())
	return i->second;
      else
	return NULL;
    }
  
    
    /**
     * Search the transition pattern list for a successor by name.
     *
     * @param patterName The name of the pattern to be found.
     * @return A pointer to the pattern,
     * or NULL if no pattern with this name is found.
     *
     */
    Pattern * const
    Policy::getTransitionPatternPattern(const std::string& _patternName) 
    {
      PatternMap::const_iterator i = transitionPatternTable_.find(_patternName);
      return (i != transitionPatternTable_.end())? i->second : NULL;
    }
    
    /**
     * Self transitions are noops.  Real transitions trigger the
     * transition protocol.  Unbound transitions are passed to the
     * parent or ignored if no parent is available.
     */
    void 
    Policy::sendTransitionMessage(const std::string& _message) 
    {             
      // search for a successor (policy/action pattern)               
      Pattern * ap = getTransitionPattern(_message);
      if (ap == NULL) 
	// search for a transition pattern 
	ap = getTransitionPatternPattern(_message);

      if (ap != currentPattern()) { // self transitions are noops
	if (ap != NULL) {           // real transitions trigger the transition protocol
	  ap->open(this, _message);
	}
	else {                      // unbound transitions are 
	  if (parent_ != NULL) {    // passed to the parent
	    parent_->sendTransitionMessage(_message);
	  }
	  else {                    // or ignored if no parent is available
		  std::cerr << "Pattern: no transition registered for message! \a" 
		 << std::endl;
	  }
	}
      }
    }


    /**
     * This can be done, while the policy is open, and an action pattern
     * is active. And is a first step into dynamic policy configuration.
     *
     * @param _pattern The name of the action pattern.
     * @param _behaviour The name of the behaviour within the action pattern.
     * @return A parameter struct for the behaviour. Note that the type
     * of the parameter struct is the type of the behaviours parameter
     * struct. The calller takes ownership of the BehaviourParameters
     * object.
     */
    BehaviourParameters * const
    Policy::getBehaviourParameters(std::string const& _pattern,
				   std::string const& _behaviour) const
    {
      std::string first, rest;
      splitPath(_pattern, first, rest);

      Pattern const * const pattern = getPattern(first);
      if (pattern != NULL) {
	return getBehaviourParameters(rest, _behaviour);
      }

      throw BehaviourEngine::EUnknownActionPattern();
    }

    /**
     * This can be done, while the policy is open, and an action pattern
     * is active. And is a first step into dynamic policy configuration.
     *
     * @param _pattern The name of the action pattern.
     * @param _behaviour The name of the behaviour within the action pattern.
     * @param _parameters A parameter struct for the behaviour. Note
     * that the type of the parameter struct has to match the type
     * expected by the behaviour. The policy takes ownership of the
     * BehaviourParameters object.
     */
    void
    Policy::setBehaviourParameters(std::string const& _pattern,
				   std::string const& _behaviour, 
				   BehaviourParameters * _parameters) 
    {
      std::string first, rest;
      splitPath(_pattern, first, rest);

      Pattern const * const pattern = getPattern(first);
      if (pattern != 0) {
	return setBehaviourParameters(rest, _behaviour, _parameters);
      }

      throw BehaviourEngine::EUnknownActionPattern();
    }

    void 
    Policy::printToStream(std::ostream& ostr) const
    {
      ostr << Indent::ws() << "Policy: " << getName() << std::endl
	   << Indent::ws() << "Number of Patterns: " << patterns_.size() << std::endl
	   << Indent::ws() << "Start pattern: " 
	   << ((startPattern_ != NULL)? startPattern_->getName() : "(null)")
	   << std::endl;

      Indent indent;
      Super::printToStream(ostr);
      PatternMap::const_iterator i;
      for(i = patterns_.begin(); i != patterns_.end(); ++i) {
	ostr << *(i->second);
      }
    }

    void
    Policy::lowerTransitions()
    {
      {
	TransitionMap::iterator first, last = transitions_.end();
	for (first = transitions_.begin(); first != last; ++first) {
	  Pattern * source = getPattern(first->first.first);
	  if (source == NULL) {
	    std::string error("Unknown source pattern for transition: " + 
			      first->first.first + ", " +
			      first->first.second + " --> " +
			      first->second);
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
	  }

	  Pattern * target = getPattern(first->second);
	  if (target == NULL) {
	    std::string error("Unknown target pattern for transition: " + 
			      first->first.first + ", " +
			      first->first.second + " --> " +
			      first->second);
	  throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
	  }

	  source->addTransition(first->first.second, target);
	}
      }

      {
	TransitionPatternMap::iterator first, last = transitionPatterns_.end();
	for (first = transitionPatterns_.begin(); first != last; ++first) {
	  Pattern * target = getPattern(first->second);
	  if (target == NULL) {
	    std::string error("Unknown target pattern for transition pattern: " + 
			      first->first + " --> " +
			      first->second);
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
	  }
	  
	  transitionPatternTable_[first->first] = target;
	}
      }
    }

    ActionPattern *
    Policy::currentLeafPattern() 
    {
      if (currentPattern_ != NULL)
	return currentPattern_->currentLeafPattern();
      return NULL;
    }

    void
    Policy::splitPath(std::string const& _path, 
		      std::string& _first, std::string& _rest)
    {
      std::string::size_type i = _path.find('/');

      if (i == std::string::npos) {
	_first = _path;
	_rest = "";
      }
      else {
	_first = _path.substr(0, i);
	_rest = _path.substr(i + 1);
      }
    }
  }
}
