// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BehaviourEngineC.h"

#include "Policy.h"
#include "ActionPattern.h"
#include "ArbiterRepository.h"
#include "Arbiter.h"
#include "ArbiterParameters.h"
#include "Behaviour.h"
#include "BehaviourRepository.h"
#include "BehaviourParameters.h"

#include <qdom.h>
#include <qfile.h>
#include <qstring.h>

namespace Miro
{
  using std::string;

  Policy::Policy(StructuredPushSupplier * _pSupplier) :
    pSupplier_(_pSupplier),
    startPattern_(NULL),
    valid_(false)
  {}

  /**
   * Loads policy from a file.  
   *
   * @param _parameterFile File name.
   * @param _pSupplier Pointer to a structured push supplier to send
   * events to for online supervision of the behaviour engine. If the
   * pointer is NULL, no events will be generated.
   */
  Policy::Policy(const char * _parameterFile, 
		 StructuredPushSupplier * _pSupplier) :
    pSupplier_(_pSupplier),
    startPattern_(NULL),
    valid_(false)
  {
    loadPolicyFile(_parameterFile);
  }

  Policy::~Policy()
  {
    close();
  }
		
  void 
  Policy::loadPolicyFile(const char* filename) 
  {
    QDomDocument doc( "MiroPolicyConfigFile" );
    QFile f(filename);
    if ( !f.open( IO_ReadOnly ) )
      throw BehaviourEngine::EFile();
    if ( !doc.setContent( &f ) ) {
      f.close();
      throw BehaviourEngine::EMalformedXML();
    }
    f.close();

    parsePolicy(doc);
  }

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
	    string actionPatternName(attribute.value());
	    actionPattern = new ActionPattern(actionPatternName, pSupplier_);
	    registerActionPattern(actionPattern);
	  } 
	  else {
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("ActionPattern without a name."));
	  }
	
	  attribute = e.attributeNode ("start");
	  if (!attribute.isNull()) {
	    if (string(attribute.value()) == "true")
		startPattern_ = actionPattern;
	  }	
	} 
	else {
	  std::string error("Non-actionpattern tag: " + string(e.tagName()));
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

	assert(e.tagName() == "actionpattern");

	QDomAttr attribute = e.attributeNode("name");

	assert(!attribute.isNull());

	string actionPatternName(attribute.value());
	ActionPattern*  actionPattern = getActionPattern(actionPatternName);

	assert(actionPattern != NULL);

	actionPattern->xmlInit(n, actionPatterns_);
      }
      n = n.nextSibling();
    }             

    // If we could parse it, then it's a good policy   
    valid_ = true;

    // if (debug())
    cout << *this;
  }

  void
  Policy::open()
  {
    if (!valid())
      throw BehaviourEngine::ENoPolicy();

    if (startPattern_)
      startPattern_->open();
    else
      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("No start pattern specified."));
  }

  void 
  Policy::close()
  {
    ActionPattern::closeCurrentActionPattern();
  }

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
    cout << "Policy: Registering " 
	 << _actionPattern->getActionPatternName() << endl;
    if (actionPatterns_.find(_actionPattern->getActionPatternName()) != 
	actionPatterns_.end()) {
      std::string error("Action pattern " +
			_actionPattern->getActionPatternName() +
			"already registered.");
      throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup(error.c_str()));
    }
    actionPatterns_[_actionPattern->getActionPatternName()] = _actionPattern;	
  }

  ActionPattern* Policy::getActionPattern(const string& _name) 
  {
    ActionPatternMap::const_iterator i = actionPatterns_.find(_name);
    if (i != actionPatterns_.end())
      return i->second;
    else
      return NULL;
  }

  void 
  Policy::printToStream(std::ostream& ostr) const
  {
    cout << "Printing policy. Patterns: " << actionPatterns_.size() << endl; 
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
