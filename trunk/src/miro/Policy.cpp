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

#include "idl/BehaviourEngineC.h"

#include "Policy.h"
#include "ActionPattern.h"
#include "ArbiterRepository.h"
#include "Arbiter.h"
#include "ArbiterParameters.h"
#include "Behaviour.h"
#include "BehaviourRepository.h"
#include "BehaviourParameters.h"
#include "Log.h"

#include <qdom.h>
#include <qfile.h>
#include <qstring.h>

namespace Miro
{
  using std::string;

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
  {}

  /**
   * Loads policy from a file.  
   *
   * @param _parameterFile File name.
   * @param _pSupplier Pointer to a structured push supplier to send
   * transition events to the notification channel for online
   * supervision of the behaviour engine. If the pointer is NULL, no
   * events will be generated.
   */
  Policy::Policy(const char * _parameterFile, 
		 StructuredPushSupplier * _pSupplier) :
    pSupplier_(_pSupplier),
    startPattern_(NULL),
    currentPattern_(NULL),
    valid_(false)
  {
    loadPolicyFile(_parameterFile);
  }

  Policy::~Policy()
  {
    close();
  }
	
  /** 
   * @param _filename The name of the file to load.
   */
  void 
  Policy::loadPolicyFile(const char* _filename) 
  {
    QDomDocument doc( "MiroPolicyConfigFile" );
    QFile f(_filename);
    if ( !f.open( IO_ReadOnly ) )
      throw BehaviourEngine::EFile();
    if ( !doc.setContent( &f ) ) {
      f.close();
      throw BehaviourEngine::EMalformedXML();
    }
    f.close();

    parsePolicy(doc);
  }

  /**
   * @param _policy The string has to contain a valid XML description
   * of the policy.
   */
  void 
  Policy::loadPolicy(const char * _policy) 
  {
    QDomDocument doc( "MiroPolicyConfigFile" );
    QCString p(_policy);
    if (!doc.setContent(p)) {
      throw BehaviourEngine::EMalformedXML();
    }

    parsePolicy(doc);
  }

  void
  Policy::parsePolicy(const QDomDocument& _doc)
  {
    clear();

    QDomElement docElem = _doc.documentElement();

    QDomNode n = docElem.firstChild();

    // first pass
    // collect all action patterns
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) {            // the node was really an element.
	if (e.tagName() == "actionpattern") {
	  ActionPattern* actionPattern;
	  QDomAttr attribute = e.attributeNode("name");
	  if (!attribute.isNull() && !attribute.value().isEmpty()) {
	    string actionPatternName(attribute.value().latin1());
	    actionPattern = new ActionPattern(actionPatternName, pSupplier_);
	    registerActionPattern(actionPattern);
	  } 
	  else {
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("ActionPattern without a name."));
	  }
	
	  attribute = e.attributeNode("start");
	  if (!attribute.isNull()) {
	    if (attribute.value() == "true")
		startPattern_ = actionPattern;
	  }	
	} 
	else {
	  std::string error("Non-actionpattern tag: " + 
		            string(e.tagName().latin1()));
	  throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
	}
      }
      n = n.nextSibling();
    }                	

    if (startPattern_ == NULL)
      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Non start pattern specified."));
  
    // second pass
    // parse action pattern data
    n = docElem.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) {            // the node was really an element.

	MIRO_ASSERT(e.tagName() == "actionpattern");

	QDomAttr attribute = e.attributeNode("name");

	MIRO_ASSERT(!attribute.isNull());

	string actionPatternName(attribute.value().latin1());
	ActionPattern*  actionPattern = getActionPattern(actionPatternName);

	MIRO_ASSERT(actionPattern != NULL);

	actionPattern->xmlInit(n, actionPatterns_);
      }
      n = n.nextSibling();
    }             

    // If we could parse it, then it's a good policy   
    valid_ = true;

    MIRO_DBG_OSTR(MIRO, LL_DEBUG, *this);
  }

  /**
   * This will activate the actionpattern indicated as
   * start pattern within the policy configuration.
   * 
   */
  void
  Policy::open()
  {
    if (!valid())
      throw BehaviourEngine::ENoPolicy();

    if (startPattern_) {
      Guard guard(transitionMutex_);
      startPattern_->open();
    }
    else
      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("No start pattern specified."));
  }
  
  /**
   * This will activate an actionpattern within the policy,
   * deactivating any previously active pattern (if any).
   */
  void 
  Policy::openActionPattern(const std::string& _pattern)
  {
    ActionPattern * pattern = getActionPattern(_pattern);
    if (pattern) {
      Guard guard(transitionMutex_);
      pattern->open();
    }
    else
      throw BehaviourEngine::EUnknownActionPattern();
  }

  /**
   * Closes the currently running action pattern
   */
  void 
  Policy::close()
  {
    Guard guard(transitionMutex_);
    if (currentPattern_) {
      currentPattern_->close(NULL);
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

    ActionPatternMap::iterator first, last = actionPatterns_.end();
    for (first = actionPatterns_.begin(); first != last; ++first)
      delete first->second;
    actionPatterns_.clear();
    valid_ = false;
  }

  void
  Policy::registerActionPattern(ActionPattern * _actionPattern) 
  {
    std::cout << "Policy: Registering " 
	 << _actionPattern->getActionPatternName() << std::endl;
    if (!actionPatterns_.insert(std::make_pair(_actionPattern->getActionPatternName(),
					       _actionPattern)).second) {
      std::string error("Action pattern " +
			_actionPattern->getActionPatternName() +
			"already registered.");
      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
    }
    _actionPattern->policy_ = this;
  }
  
  /**
   * @ param _name The name of the action pattern.
   * @return A pointer to the action pattern or NULL
   * if it does not exist.
   */
  ActionPattern * 
  Policy::getActionPattern(const std::string& _name) 
  {
    ActionPatternMap::const_iterator i = actionPatterns_.find(_name);
    if (i != actionPatterns_.end())
      return i->second;
    else
      return NULL;
  }
  
  /**
   * @ param _name The name of the action pattern.
   * @return A pointer to the action pattern or NULL
   * if it does not exist.
   */
  ActionPattern const * const 
  Policy::getActionPattern(const std::string& _name) const
  {
    ActionPatternMap::const_iterator i = actionPatterns_.find(_name);
    if (i != actionPatterns_.end())
      return i->second;
    else
      return NULL;
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
  Policy::getBehaviourParameters(const std::string& _pattern,
				 const std::string& _behaviour) const
  {
    ActionPattern const * const pattern = getActionPattern(_pattern);

    if (pattern) 
      return pattern->getBehaviourParameters(_behaviour);
    
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
  Policy::setBehaviourParameters(const std::string& _pattern,
				 const std::string& _behaviour, 
				 BehaviourParameters * _parameters) 
  {
    ActionPattern * pattern = getActionPattern(_pattern);

    if (pattern) {
      pattern->setBehaviourParameters(_behaviour, _parameters);
    }
    else 
      throw BehaviourEngine::EUnknownActionPattern();
  }

  void 
  Policy::printToStream(std::ostream& ostr) const
  {
    ostr << "Printing policy. Patterns: " << actionPatterns_.size() << std::endl; 
    ActionPatternMap::const_iterator i;
    for(i = actionPatterns_.begin(); i != actionPatterns_.end(); ++i) {
      ostr << *(i->second);
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const Policy& _policy)
  {
    _policy.printToStream(ostr);

    return ostr;
  }
};
