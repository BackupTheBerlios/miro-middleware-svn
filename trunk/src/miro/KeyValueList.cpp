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

#include "KeyValueList.h"
#include "BehaviourEngineC.h"

#include <qdom.h>

namespace Miro
{
  void
  KeyValueList::addPair(const QString& _name, const QString& _value) 
  {
    parameters_[_name] = _value;	
  }

  QString
  KeyValueList::getValue(const QString& _name) const
  {
    KeyValueListMap::const_iterator i = parameters_.find(_name);
    return (i != parameters_.end())? i->second : QString();
  }

  void 
  KeyValueList::printToConsole() 
  {
    for(KeyValueListMap::const_iterator i = parameters_.begin(); i != parameters_.end(); ++i) {
      cout << i->first << "=\"" << i->second << "\"" << endl;
    }
  }

  void
  KeyValueList::operator<<=(const QDomNode& _node)
  {
    QDomNode n = _node.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) {            // the node was really an element.
	if (e.tagName() == "parameter") {
	  QDomAttr parameterName = e.attributeNode("name");
	  QString name;
	  if (!parameterName.isNull() && !parameterName.value().isEmpty()) {
	    name = parameterName.value();
	  } 
	  else {
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Parameter tag without name."));
	  }

	  QDomAttr parameterValue = e.attributeNode("value");
	  QString value;
	  if (!parameterValue.isNull() && !parameterValue.value().isEmpty()) {
	    value = parameterValue.value();
	  } 
	  else {
	    throw BehaviourEngine::EMalformedPolicy(CORBA::string_dup("Parameter tag without value."));
	  }

	  addPair(name, value);
	}
      }
      n = n.nextSibling();
    }
  }
};
