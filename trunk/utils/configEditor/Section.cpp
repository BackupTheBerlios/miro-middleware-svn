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

#include "Section.h"
#include "ParameterInstance.h"
#include "ParameterSingleton.h"
#include "ConfigDocumentXML.h"

#include "utils/widgets/ConfigFile.h"

#include "miro/Log.h"
#include "params/Generator.h"
#include "params/Type.h"

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qinputdialog.h>
#include <qmessagebox.h>

#include <algorithm>
#include <iostream>

const QString Section::XML_TAG("section");

Section::Section(QDomNode const& _node,
		 QListViewItem * _parentItem, QListViewItem * _pre,
		 QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name),
  menuAddParameter_(NULL),
  menuAddInstance_(NULL)
{
  listViewItem()->setText(2, className());

  MIRO_ASSERT(!node_.isNull());

  buildSubtree();
}

void
Section::contextMenu(QPopupMenu& _menu)
{
  // get all current parameters
  Miro::CFG::Generator::QStringVector childParameters;
  QListViewItem * item = listViewItem()->firstChild();
  while (item != NULL) {
    ItemXML * itemXML =
      dynamic_cast<ItemXML *>(Item::itemMap().find(item)->second);
    MIRO_ASSERT(itemXML != NULL);

    QDomElement e = itemXML->node().toElement();
    MIRO_ASSERT(!e.isNull());

    if (e.tagName() == ParameterXML::XML_TAG) {
      childParameters.push_back(item->text(0));
    }
    item = item->nextSibling();
  }

  typedef std::vector<QString> QStringVector;

  QStringVector paramsList;
  QStringVector instanceList;

  QString section = listViewItem()->text(0);

  Miro::CFG::Generator::GroupMap::const_iterator first, last;
  ConfigFile::instance()->description().getGroupedTypes(section, first, last);

  for (; first != last; ++first) {
    if (first->second.isFinal()) {
      QString name = first->second.fullName();
      if (first->second.instance()) {
	Miro::CFG::Generator::QStringVector::const_iterator i =
	  std::find(childParameters.begin(), childParameters.end(), name);
	if (i == childParameters.end()) {
	  paramsList.push_back(name);
	}
      }
      else {
	instanceList.push_back(name);
      }
    }
  }

  // create a new parameter selection menu
  menuAddParameter_ = new QPopupMenu(&_menu);
  // create a new instance selection menu
  menuAddInstance_ = new QPopupMenu(&_menu);

  {
    std::sort(paramsList.begin(), paramsList.end());
    std::sort(instanceList.begin(), instanceList.end());
    QStringVector::const_iterator first, last = paramsList.end();
    for (first = paramsList.begin(); first != last; ++first) {
      menuAddParameter_->insertItem(*first);
    }
    last = instanceList.end();
    for (first = instanceList.begin(); first != last; ++first) {
      menuAddInstance_->insertItem(*first);
    }
  }


  connect(menuAddInstance_, SIGNAL(activated(int)), 
	  this, SLOT(onAddInstance(int)));
  connect(menuAddParameter_, SIGNAL(activated(int)), 
	  this, SLOT(onAddParameter(int)));

  _menu.insertItem("Add Parameter", menuAddParameter_);
  _menu.insertItem("Add Instance", menuAddInstance_);
  _menu.insertSeparator();
  _menu.insertItem("Up", this, SLOT(up()));
  _menu.insertItem("Down", this, SLOT(down()));
  _menu.insertSeparator();
  _menu.insertItem("Delete", this, SLOT(slotDelete()));
}

void
Section::onAddInstance(int _n)
{
  bool ok = false;
  QString name = QInputDialog::getText(tr( "Parameter Instance" ),
					    tr( "Instance name:" ),
					    QLineEdit::Normal, NULL, &ok, NULL );
  if ( ok && !name.isEmpty()) {
    
    QDomDocument document = node_.ownerDocument();
    QDomElement e = document.createElement(ParameterInstance::XML_TAG);
    e.setAttribute(ParameterInstance::XML_ATTRIBUTE_KEY, name);
    e.setAttribute(ParameterInstance::XML_ATTRIBUTE_TYPE, menuAddInstance_->text(_n));
    
    QDomNode n = node_.firstChild();
    QDomNode newChild = node_.insertBefore(e, n);
    
    MIRO_ASSERT(!newChild.isNull());
    try {
      new ParameterInstance(listViewItem(), NULL, newChild, 
			    this, name);
    }
    catch (QString const& e) {
      QMessageBox::warning(NULL, 
			   "Error constructing parameter instance:",
			   e);
    }
    setModified();
  }
}

void
Section::onAddParameter(int _n)
{
  QDomDocument document = node_.ownerDocument();
  QDomElement e = document.createElement(ParameterXML::XML_TAG);
  e.setAttribute(ParameterXML::XML_ATTRIBUTE_KEY, menuAddParameter_->text(_n));

  QDomNode n = node_.firstChild();
  QDomNode newChild = node_.insertBefore(e, n);
    
  MIRO_ASSERT(!newChild.isNull());
  try {
    new ParameterSingleton(listViewItem(), NULL, newChild, 
			   this,  menuAddParameter_->text(_n));
  }
  catch (QString const& e) {
    QMessageBox::warning(NULL, 
			 "Error constructing parameter singleton:",
			 e);
  }
  setModified();
}

void
Section::buildSubtree()
{
  QDomNode n = node_.firstChild();
  QListViewItem * pre = NULL;
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull()) {
      CompoundParameter * param = NULL;
      try {
	if (e.tagName() == ParameterInstance::XML_TAG &&
	    e.hasAttribute(XML_ATTRIBUTE_KEY)) {
	  param =
	    new ParameterInstance(listViewItem(), pre,
				  e, this, e.attribute(XML_ATTRIBUTE_KEY));
	}
	if (e.tagName() == ParameterXML::XML_TAG &&
	    e.hasAttribute(XML_ATTRIBUTE_KEY)) {
	  param =
	    new ParameterSingleton(listViewItem(), pre,
				   e, this, 
				   e.attribute(XML_ATTRIBUTE_KEY));
	}
	if (param != NULL) {
	  param->init();
	  pre = param->listViewItem();
	}
      }
      catch (QString const& e) {
	QMessageBox::warning(NULL, 
			     "Error constructing parameter item:",
			     e);
	break;
      }
    }
    n = n.nextSibling();
  }
}
