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

#include "ActionPatternXML.h"
#include "BehaviourXML.h"
#include "ArbiterXML.h"
#include "TransitionXML.h"
#include "SubpolicyXML.h"
#include "PatternWidget.h"

#include "miro/Log.h"
#include "params/Generator.h"
#include "utils/widgets/ConfigFile.h"

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qinputdialog.h>

QString const ActionPatternXML::XML_TAG("actionpattern");

ActionPatternXML::ActionPatternXML(QDomNode const& _node,
				   QListViewItem * _parentItem, QListViewItem * _pre,
				   QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name),
  menuSetArbiter_(NULL),
  menuAddBehaviour_(NULL)
{
  listViewItem()->setText(2, "ActionPattern");
  buildSubtree();
}

bool 
ActionPatternXML::hasBehaviour(QString const& _name) const
{
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    BehaviourXML * behaviour = dynamic_cast<BehaviourXML *>(item);
    if (behaviour != NULL &&
	behaviour->name() == _name) {
      return true;
    }
    i = i->nextSibling();
  }
  return false;
}

void
ActionPatternXML::contextMenu(QPopupMenu& _menu)
{
  // build add behaviour submenu
  menuAddBehaviour_ = new QPopupMenu(&_menu);

  // add all behaviour names //
  QStringList behaviours;
  {
    Miro::CFG::GroupMap::const_iterator first, last;
    ConfigFile::instance()->description().getGroupedTypes("Behaviour", first, last);
    for (; first != last; ++first) {
      if (first->second.isFinal() &&
	  !hasBehaviour(first->second.name())) {
	behaviours.append(first->second.name());
      }
    }
  }
  behaviours.sort();
  {
    QStringList::const_iterator first, last = behaviours.end();
    for (first = behaviours.begin(); first != last; ++first)
      menuAddBehaviour_->insertItem(*first);
  }

  connect(menuAddBehaviour_, SIGNAL(activated(int)), this, SLOT(addBehaviour(int)));

  // build set arbiter submenu
  menuSetArbiter_ = new QPopupMenu(&_menu);

  // add all behaviour names //
  QStringList arbiters;
  {
    Miro::CFG::GroupMap::const_iterator first, last;
    ConfigFile::instance()->description().getGroupedTypes("Arbiter", first, last);
    for (; first != last; ++first) {
      if (first->second.isFinal()) {
	arbiters.append(first->second.name());
      }
    }
  }
  arbiters.sort();
  {
    QStringList::const_iterator first, last = arbiters.end();
    for (first = arbiters.begin(); first != last; ++first)
      menuSetArbiter_->insertItem(*first);
  }

  connect(menuSetArbiter_, SIGNAL(activated(int)), this, SLOT(setArbiter(int)));

  _menu.insertItem("Add Behaviour", menuAddBehaviour_);
  _menu.insertItem("Set Arbiter", menuSetArbiter_);

  Super::contextMenu(_menu);
}

void
ActionPatternXML::setArbiter(int _id)
{
  // delete previous arbiter
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    ArbiterXML * arbiter = dynamic_cast<ArbiterXML *>(item);
    if (arbiter != NULL) {
      arbiter->deleteLater();
      break;
    }
    i = i->nextSibling();
  }

  // add Arbiter
  QDomElement e = 
    node_.ownerDocument().createElement(ArbiterXML::XML_TAG);
  e.setAttribute(XML_ATTRIBUTE_KEY, menuSetArbiter_->text(_id));
  QDomNode n = node_.insertBefore(e, QDomNode());
  MIRO_ASSERT(!n.isNull());

  new ArbiterXML(n, 
		 listViewItem(), NULL,
		 this, menuSetArbiter_->text(_id));
  if (widget_) {
    PatternWidget * w = dynamic_cast<PatternWidget *>(widget_);
    MIRO_ASSERT(w != NULL);
    w->init();
  }
  setModified();
}

void
ActionPatternXML::addBehaviour(int _id)
{
  QDomElement e = 
    node_.ownerDocument().createElement(BehaviourXML::XML_TAG);
  e.setAttribute(XML_ATTRIBUTE_KEY, menuAddBehaviour_->text(_id));
  QDomNode n = node_.insertBefore(e, QDomNode());
  MIRO_ASSERT(!n.isNull());

  new BehaviourXML(n, 
		   listViewItem(), NULL,
		   this, menuAddBehaviour_->text(_id));
  if (widget_) {
    PatternWidget * w = dynamic_cast<PatternWidget *>(widget_);
    MIRO_ASSERT(w != NULL);
    w->init();
  }
  setModified();
}

void 
ActionPatternXML::buildSubtree() 
{
  QDomNode n = node_.firstChild();
  QListViewItem * pre = NULL;
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull()) {
      Item * item = NULL;
      try {
	if (e.tagName() == BehaviourXML::XML_TAG &&
	    e.hasAttribute(XML_ATTRIBUTE_KEY)) {
	  item =
	    new BehaviourXML(e,
			     listViewItem(), pre,
			     this, e.attribute(XML_ATTRIBUTE_KEY));
	}
	else if (e.tagName() == ArbiterXML::XML_TAG &&
	    e.hasAttribute(XML_ATTRIBUTE_KEY)) {
	  item =
	    new ArbiterXML(e,
			   listViewItem(), pre,
			   this, e.attribute(XML_ATTRIBUTE_KEY));
	}
	else if (e.tagName() == TransitionXML::XML_TAG &&
	    e.hasAttribute(TransitionXML::XML_ATTRIBUTE_MSG)) {
	  item =
	    new TransitionXML(e,
			      listViewItem(), pre,
			      this, e.attribute(TransitionXML::XML_ATTRIBUTE_MSG));
	}
	if (item != NULL) {
	  ParameterXML * param = dynamic_cast<ParameterXML *>(item);
	  if (param)
	    param->init();
	  pre = item->listViewItem();
	}
      }
      catch (QString const& e) {
	QMessageBox::warning(NULL, 
			     "Error constructing policy item:",
			     e);
	break;
      }
    }
    n = n.nextSibling();
  }
}

