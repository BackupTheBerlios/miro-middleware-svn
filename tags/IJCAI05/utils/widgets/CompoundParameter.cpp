// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "CompoundParameter.h"
#include "SimpleParameter.h"
#include "ParameterList.h"

#include "ParameterDialog.h"
#include "ConfigFile.h"

#include "params/Type.h"
#include "params/Generator.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qlistview.h>

CompoundParameter::CompoundParameter(Miro::CFG::Type const& _type,
				     QDomNode const& _node,
				     QListViewItem * _parentItem,
				     QListViewItem * _pre,
				     QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name),
  type_(_type)
{
  if (listViewItem()->listView()->columns() >= 3)
    listViewItem()->setText(2, type_.fullName());
}

CompoundParameter::CompoundParameter(Miro::CFG::Type const& _type,
				     QDomNode const& _node,
				     QListView * _list,
				     QListViewItem * _pre,
				     QObject * _parent, const char * _name) :
  Super(_node, _list, _pre, _parent, _name),
  type_(_type)
{
  if (listViewItem()->listView()->columns() >= 3)
    listViewItem()->setText(2, type_.fullName());
}

void
CompoundParameter::init()
{
  // get complete parameter set including super classes
  Miro::CFG::ParameterVector params = config_->description().getFullParameterSet(type_);

  QDomNode n = node().firstChild();
  QListViewItem * pre = NULL;
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull() &&
	e.tagName() == ParameterXML::XML_TAG) {
      ParameterXML * newParam = NULL;
      if (!e.hasAttribute(ParameterXML::XML_ATTRIBUTE_KEY))
	throw QString("Parameter tag without name in compund (" + 
		      type_.fullName() + ") " + name());

      QString p = e.attribute(ParameterXML::XML_ATTRIBUTE_KEY);
      QString pLower = p;

      // we need a lower case first letter version for comparison
      pLower[0] = p[0].lower();
      Miro::CFG::ParameterVector::const_iterator i, last = params.end();
      for (i = params.begin(); i != last; ++i) {
	if (i->name_ == pLower) {
	  break;
	}
      }
      if (i == last)
	throw QString("Parameter " + p + " unknown in compund (" + 
		      type_.fullName() + ") " + name());

      if (SimpleParameter::typeFromName(i->type_) != 
	  SimpleParameter::NONE) {
	newParam = new SimpleParameter(*i, n, listViewItem(), pre, this, p);
      }
      else if (ParameterList::typeFromName(i->type_) !=
	       ParameterList::NONE) {
	newParam = new ParameterList(*i, n, listViewItem(), pre, this, p);
      }
      else {
	Miro::CFG::Type const * const t = 
	  config_->description().getType(i->type_);
	if (t != NULL) {
	  newParam = new CompoundParameter(*t, 
					   n, listViewItem(), pre,
					   this, p);
				       
	}
	else
	  throw QString("Type " + i->type_ + " of parameter " + p +
			" unknown in compund (" + 
			type_.fullName() + ") " + name());
      }
      if (newParam != NULL)
	newParam->init();
    }
    n = n.nextSibling();
  }
}

void
CompoundParameter::setParameters()
{
  ParameterDialog dialog(type_,
			 node_.parentNode(), node_, 
			 NULL, this,
			 NULL, name());
      
  int rc = dialog.exec();
  if (rc == QDialog::Accepted) {
    dialog.setXML();
    if (dialog.modified())
      setModified(true);
  }
}

Miro::CFG::Type const&
CompoundParameter::type(QDomNode const& _node, QString const& _attribute)
{
  QDomNode n = _node;
  QDomElement e = n.toElement();
  QString typeName = e.attribute(_attribute);

  Miro::CFG::Type const * const parameterType =
    ConfigFile::instance()->description().getType(typeName);
  
  if (parameterType == NULL) {
    throw QString("Parameter description for " + typeName +
		  " not found.\nCheck whether the relevant description file is loaded.");
  }
  return *parameterType;
}
