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

#include "Policy.h"
#include "ActionPattern.h"
#include "ArbiterRepository.h"
#include "Arbiter.h"
#include "ArbiterParameters.h"
#include "Behaviour.h"
#include "BehaviourRepository.h"
#include "BehaviourParameters.h"
#include "KeyValueList.h"
#include "Exception.h"

#include <qdom.h>
#include <qfile.h>
#include <qstring.h>

using std::string;
using Miro::Exception;

namespace Miro
{
  Policy::Policy(StructuredPushSupplier * _pSupplier) :
    pSupplier_(_pSupplier),
    behaviourRepository_(BehaviourRepository::instance()),
    arbiterRepository_(ArbiterRepository::instance()),
    startPattern_(NULL)
  {}

  Policy::Policy(const char* parameterFile, 
		 StructuredPushSupplier * _pSupplier) :
    pSupplier_(_pSupplier),
    behaviourRepository_(BehaviourRepository::instance()),
    arbiterRepository_(ArbiterRepository::instance()),
    startPattern_(NULL)
  {
    loadActionPatterns(parameterFile);
  }

  Policy::~Policy()
  {
  }
		
  void 
  Policy::loadActionPatterns(const char* filename) 
  {
    QDomDocument doc( "MiroPolicyConfigFile" );
    QFile f(filename);
    if ( !f.open( IO_ReadOnly ) )
      return;
    if ( !doc.setContent( &f ) ) {
      f.close();
      return;
    }
    f.close();

    // print out the element names of all elements that are a direct child
    // of the outermost element.

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) { // the node was really an element.
	if (e.tagName()=="actionpattern") {
	  ActionPattern* actionPattern;
	  QDomAttr attribute = e.attributeNode("name");
	  if (!attribute.isNull()) {
	    string actionPatternName = string(attribute.value());
	    actionPattern = new ActionPattern(actionPatternName, pSupplier_);
	    retrieveArbiters(*actionPattern, n);
	    retrieveBehaviours(*actionPattern, n);
	    registerActionPattern(actionPattern);
	    
	    if (actionPattern->arbiter() == NULL)
	      throw Exception("ActionPattern without an arbiter: " + actionPatternName);
	  } else {
	    throw Exception("ActionPattern without a name");
	  }
	
	  attribute = e.attributeNode ("start");
	  if (!attribute.isNull()) {
	    if (string(attribute.value()) == "true")
		startPattern_ = actionPattern;
	  }
	
	} else {
	  throw Exception("Non-actionpattern tag: " + string(e.tagName()));
	}
      }
      n = n.nextSibling();
    }                	
  
    n = docElem.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) { // the node was really an element.
	if (e.tagName()=="actionpattern") {
	  QDomAttr attribute = e.attributeNode("name");
	  if (!attribute.isNull()) {
	    string actionPatternName = string(attribute.value());
	    ActionPattern* actionPattern = getActionPattern(actionPatternName);
	    retrieveTransitions(*actionPattern, n);
	  } else {
	    throw Exception("ActionPattern without name");
	  }
	} else {
	  throw Exception("Non-actionpattern tag: " + string(e.tagName()));
	}
      }
      n = n.nextSibling();
    }
    
    if (startPattern_ == NULL)
      throw Exception("Non start pattern specified.");
  }

  void
  Policy::open()
  {
    startPattern_->open();
  }

  void 
  Policy::close()
  {
    ActionPattern::closeCurrentActionPattern();
  }

  void 
  Policy::retrieveBehaviours(ActionPattern &_actionPattern, 
				    const QDomNode& _domElement) 
  {
    QDomNode n = _domElement.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) { // the node was really an element.
	if (e.tagName()=="behaviour") {
	  QDomAttr attribute = e.attributeNode("name");
	  if (!attribute.isNull()) {
	    string name(attribute.value());

	    Behaviour* behaviour;
	    BehaviourParameters * parameters;
	    if ((behaviour = behaviourRepository_->getBehaviour(name)) != 0) {
	      parameters = behaviour->getParametersInstance();

	      KeyValueList params;
	      retrieveBehaviourParameters(params, n);

	      *parameters <<= params;
	      _actionPattern.addBehaviour(behaviour, parameters);
	    } else {
	      throw Exception("Behaviour not registered: " + name);
	    }
	  } else {
	    throw Exception("Behaviour without name");		
	  }
	}
      }
      n = n.nextSibling();
    }
  }

  void 
  Policy::retrieveArbiters(ActionPattern &_actionPattern, 
				  const QDomNode& _domElement) 
  {
    QDomNode n = _domElement.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) { // the node was really an element.
	if (e.tagName()=="arbiter") {
	  QDomAttr attribute = e.attributeNode("name");
	  if (!attribute.isNull()) {
	    string name(attribute.value());
	    Arbiter* arbiter;
	    if ((arbiter = arbiterRepository_->getArbiter(name)) != 0) {
	      ArbiterParameters * params = arbiter->getParametersInstance();
	      _actionPattern.arbiter(arbiter, params);
	    } else {
	      throw Exception("Arbiter not registered: " + name);
	    }
	  } else {
	    throw Exception("Arbiter without name");		
	  }
	}
      }
      n = n.nextSibling();
    }
  }

  void 
  Policy::retrieveBehaviourParameters(KeyValueList &_parameter, 
					     const QDomNode& _domElement) 
  {
    QDomNode n = _domElement.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) { // the node was really an element.
	if (e.tagName()=="parameter") {
	  QDomAttr parameterName = e.attributeNode("name");
	  QString name;
	  if (!parameterName.isNull()) {
	    name = parameterName.value();
	  } else {
	    throw Exception("parameter tag without name");
	  }

	  QDomAttr parameterValue = e.attributeNode("value");
	  QString value;
	  if (!parameterValue.isNull()) {
	    value = parameterValue.value();
	  } else {
	    throw Exception("parameter tag without value");
	  }

	  _parameter.addPair(name, value);
	}
      }
      n = n.nextSibling();
    }
  }

  void 
  Policy::retrieveTransitions(ActionPattern& _actionPattern, 
				     const QDomNode& _domElement) 
  {
    QDomNode n = _domElement.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) { // the node was really an element.
	if (e.tagName()=="transition") {
	  QDomAttr attrMessage = e.attributeNode("message");
	  string message;
	  ActionPattern * actionPattern;

	  if (!attrMessage.isNull()) {
	    message = string(attrMessage.value());
	  } else {
	    throw Exception("transition without message");
	  }

	  QDomAttr attrPattern = e.attributeNode("target");
	  if (!attrPattern.isNull()) {
	    string patternname = string(attrPattern.value());
	    actionPattern = getActionPattern(patternname);

	    if (actionPattern == 0) {
	      throw Exception("ActionPattern not registered: " + patternname);
	    }
	  }
		
	  _actionPattern.addTransition(message, actionPattern);
	}
      }
      n = n.nextSibling();
    }
  }

  void
  Policy::registerActionPattern(ActionPattern * _actionPattern) 
  {
    cout << "Policy: Registering " 
	 << _actionPattern->getActionPatternName() << endl;
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
    ActionPatternMap::const_iterator i;
    for(i = actionPatterns_.begin(); i != actionPatterns_.end(); ++i) {
      ostr << *(i->second);
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const Policy& _factory)
  {
    _factory.printToStream(ostr);

    return ostr;
  }
};
