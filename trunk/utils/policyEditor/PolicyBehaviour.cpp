// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PolicyBehaviour.h"
#include "PolicyDocument.h"

PolicyBehaviour::PolicyBehaviour(PolicyDocumentClass& _document,
				 QDomNode _node) :
  document_(_document),
  node_(_node)
{
}


//---------------------- parameter methods ---------------------------//

// returns a stringmap with all parameters of the given behaviour in
// the given pattern.
QMap<QString, QString>
PolicyBehaviour::getParameters() const
{
  QMap<QString, QString> stringMap;

  // check for each child: if it is a parameter //
  QDomNode n = node_.firstChild();
  while(!n.isNull()) {
    QDomElement e = n.toElement();
    // if tagName == "parameter" -> add to stringMap //
    if ( e.tagName() == XML_TAG_PARAMETER) {
    
      QString name  = e.attribute(XML_PARAM_KEY, "");
      QString value = e.attribute(XML_PARAM_VALUE, "");

      stringMap.insert(name, value);
    }

    n = n.nextSibling();
  }
  
  return stringMap;
}


// sets a new set of parameters for the given behaviour in the given //
// pattern. All existing parameters are deleted!                     //
void 
PolicyBehaviour::setParameters(const QMap<QString, QString>& paramMap)
{
  // delete current parameters //
  delParameters();

  // add all parameters in the paramMap //
  QMap<QString, QString>::ConstIterator first, last = paramMap.end();
  for (first = paramMap.begin(); first != last; ++last) {
    addParameter(first.key(), first.data());
  }

  document_.setModified();
}


// adds a parameter to the given behaviour in the given pattern //
void 
PolicyBehaviour::addParameter(const QString& key, const QString& value)
{
  // create new DOM element //
  QDomElement element = node_.ownerDocument().createElement(XML_TAG_PARAMETER);

  // set attributes //
  element.setAttribute(XML_PARAM_KEY, key);
  element.setAttribute(XML_PARAM_VALUE, value);

  // append element to main node //
  node_.appendChild(element);
}


// deletes all parameters of the given behaviour in the given pattern //
void
PolicyBehaviour::delParameters()
{
  // check for each child: if it is a parameter //
  QDomNode n = node_.firstChild();
  while(!n.isNull()) {

    QDomNode tmp = n;
    n.nextSibling();

    // if tagName == "parameter" -> remove node //
    if (tmp.toElement().tagName() == XML_TAG_PARAMETER) {
      node_.removeChild(tmp);
    }
  }
}
