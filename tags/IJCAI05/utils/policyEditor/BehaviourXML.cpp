// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BehaviourXML.h"
#include "BehaviourWidget.h"
#include "PatternWidget.h"
#include "utils/widgets/ConfigFile.h"
#include "params/Generator.h"

#include <qpopupmenu.h>
#include <qlistview.h>

#include <iostream>
using namespace std;

QString const BehaviourXML::XML_TAG("behaviour");

BehaviourXML::BehaviourXML(QDomNode& _node,
			   QListViewItem * _parentItem, QListViewItem * _pre,
			   QObject * _parent, const char * _name) :
  Super(type(_node, XML_ATTRIBUTE_KEY),
	_node, _parentItem, _pre, _parent, _name)
{
  listViewItem()->setText(2, "Behaviour");
}

void
BehaviourXML::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Set Parameters", this, SLOT(slotSetParameters()));
  _menu.insertSeparator();
  _menu.insertItem("Up", this, SLOT(up()));
  _menu.insertItem("Down", this, SLOT(down()));
  _menu.insertSeparator();
  _menu.insertItem("Delete", this, SLOT(slotDelete()));



  cout << config_->description() << endl;
}

QStringVector
BehaviourXML::transitionMessages() const
{
  QStringVector v;

  // get complete static const parameter set including super classes
  Miro::CFG::ParameterVector params = 
    config_->description().getFullStaticConstParameterSet(type_);
  
  v.reserve(params.size());

  Miro::CFG::ParameterVector::const_iterator first, last = params.end();
  for (first = params.begin(); first != last; ++first) {
    if (first->type_ == "Miro::BAP::TransitionMessage") {
      v.push_back(first->default_);
    }
  }

  cout << "Transition messages of " << type_.name() << endl;
  for (unsigned int i = 0; i < v.size(); ++i) {
    cout << "  " << v[i] << endl;
  }
    
  return v;
}

#ifdef OLD_CODE

//---------------------- parameter methods ---------------------------//

// returns a stringmap with all parameters of the given behaviour in
// the given pattern.
QMap<QString, QString>
BehaviourXML::getParameters() const
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
BehaviourXML::setParameters(const QMap<QString, QString>& paramMap)
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
BehaviourXML::addParameter(const QString& key, const QString& value)
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
BehaviourXML::delParameters()
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
#endif
