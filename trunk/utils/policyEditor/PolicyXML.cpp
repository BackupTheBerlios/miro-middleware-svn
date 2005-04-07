// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PolicyXML.h"
#include "SubpolicyXML.h"
#include "DownTransitionXML.h"
#include "ActionPatternXML.h"
#include "PolicyDocumentXML.h"
#include "PolicyView.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qstringlist.h>
#include <qobjectlist.h>

#include <algorithm>
#include <iostream>

using namespace std;

QString const PolicyXML::XML_TAG("policy");

PolicyXML::PolicyXML(QDomNode const& _node,
		     QListViewItem * _parentItem, QListViewItem * _pre,
		     QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name),
  menuAddSubpolicy_(NULL),
  menuAddDownTransition_(NULL)
{
  listViewItem()->setText(2, "Policy");
  buildSubtree();
}

PolicyXML::~PolicyXML()
{
  // if the policy is currently displayed, reset the view
  if (widget_!= NULL) {
    PolicyView * view = dynamic_cast<PolicyView *>(widget_);
    MIRO_ASSERT(view != NULL);
    view->init(NULL);
    widget_ = NULL;
  }
}

void 
PolicyXML::policyRenaming(QString const& _oldName, QString const& _newName)
{
  QListViewItem * item = listViewItem()->firstChild();
  while (item != NULL) {
    Item * i = itemFromListViewItem(item);
    SubpolicyXML * p = dynamic_cast<SubpolicyXML *>(i);
    if (p != NULL)
      p->policyRenaming(_oldName, _newName);
    item = item->nextSibling();
  }
}

QStringList 
PolicyXML::patternList()
{
  // add all behaviour names //
  QStringList patterns;

  QDomNode n = node_.firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull() &&
	(e.tagName() == ActionPatternXML::XML_TAG ||
	 e.tagName() == SubpolicyXML::XML_TAG)) {
      MIRO_ASSERT(e.hasAttribute(XML_ATTRIBUTE_KEY));
      patterns.append(e.attribute(XML_ATTRIBUTE_KEY));
    }
    n = n.nextSibling();
  }
  patterns.sort();

  return patterns;
}

bool 
PolicyXML::uniquePatternName(QString const& _name)
{
  QDomNode n = node_.firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull() &&
	(e.tagName() == ActionPatternXML::XML_TAG ||
	 e.tagName() == SubpolicyXML::XML_TAG)) {
      MIRO_ASSERT(e.hasAttribute(XML_ATTRIBUTE_KEY));
      if (e.attribute(XML_ATTRIBUTE_KEY) == _name)
	return false;
    }
    n = n.nextSibling();
  }

  return true;
}

void
PolicyXML::removeTransitionTo(QString const& _target)
{
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    PatternXML * pattern = dynamic_cast<PatternXML *>(item);
    if (pattern != NULL) {
      pattern->removeTransitionTo(_target);
    }
    i = i->nextSibling();
  }
}

void
PolicyXML::renameTransitionTo(QString const& _oldTarget, QString const& _newTarget)
{
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    PatternXML * pattern = dynamic_cast<PatternXML *>(item);
    if (pattern != NULL) {
      pattern->renameTransitionTo(_oldTarget, _newTarget);
    }
    else {
      DownTransitionXML * transition = dynamic_cast<DownTransitionXML *>(item);
      if (transition != NULL) {
	if (transition->target() == _oldTarget)
	  transition->setTarget(_newTarget);
      }
    }
    i = i->nextSibling();
  }
}

PatternXML *
PolicyXML::patternFromName(QString const& _name)
{
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    PatternXML * pattern = dynamic_cast<PatternXML *>(item);
    if (pattern != NULL &&
	pattern->name() == _name) {
      return pattern;
    }
    i = i->nextSibling();
  }
  return NULL;
}

bool
PolicyXML::tryAddDownTransition(QString const& _msg, PatternXML * _target)
{
  if (!hasTransition(_msg)) {
    addDownTransition(_msg, _target->name());
    return true;
  }
  return false;
}

void
PolicyXML::addDownTransition(QString const& _msg, QString const& _target)
{
  QDomElement e = 
    node_.ownerDocument().createElement(DownTransitionXML::XML_TAG);
  e.setAttribute(DownTransitionXML::XML_ATTRIBUTE_MSG, _msg);
  e.setAttribute(DownTransitionXML::XML_ATTRIBUTE_TARGET,
		 _target);
  QDomNode n = node_.insertBefore(e, QDomNode());
  MIRO_ASSERT(!n.isNull());
      
  new DownTransitionXML(n, 
			listViewItem(), NULL,
			this, _msg);
  setModified();
  if (widget_) {
    PolicyView * p = dynamic_cast<PolicyView *>(widget_);
    MIRO_ASSERT(p != NULL);
    
    p->viewport()->update();
  }
}



void
PolicyXML::contextMenu(QPopupMenu& _menu)
{
  bool structuredPolicy = document()->structuredPolicy();

  // build list of available subpolicy types
  menuAddSubpolicy_ = new QPopupMenu(&_menu, "menu subpolicy");
  connect(menuAddSubpolicy_, SIGNAL(activated(int)), this, SLOT(addSubpolicy(int)));
  {
    QStringVector types = document()->policies(name());
    QStringVector::const_iterator first, last = types.end();
    for (first = types.begin(); first != last; ++first)
      menuAddSubpolicy_->insertItem(*first);
  }

  // build add downtransition submenu
  menuAddDownTransition_ = new QPopupMenu(&_menu);

  QStringList targets = patternList();
  QStringList::const_iterator first, last = targets.end();
  for (first = targets.begin(); first != last; ++first)
    menuAddDownTransition_->insertItem(*first);

  connect(menuAddDownTransition_, SIGNAL(activated(int)),
	  this, SLOT(addDownTransition(int)));

  //  Super::contextMenu(_menu);
  _menu.insertItem("Add Action Pattern", this, SLOT(addActionPattern()));
  if (structuredPolicy) {
    int i = _menu.insertItem("Add Policy Instance", menuAddSubpolicy_);
    if (menuAddSubpolicy_->count() == 0)
      _menu.setItemEnabled(i, false);
  }
  int i = _menu.insertItem("Add downward Transition", menuAddDownTransition_);
  if (menuAddDownTransition_->count() == 0)
    _menu.setItemEnabled(i, false);

  if (structuredPolicy) {
    _menu.insertSeparator();
    _menu.insertItem("Set Name", this, SLOT(slotRename()));
    _menu.insertItem("Set Start Policy", this, SLOT(setStartPattern()));
    _menu.insertItem("Up", this, SLOT(up()));
    _menu.insertItem("Down", this, SLOT(down()));
    _menu.insertSeparator();
    _menu.insertItem("Delete", this, SLOT(slotDelete()));
  }
}

void
PolicyXML::deleteItem()
{
  ItemXML::deleteItem();
}

PolicyDocumentXML *
PolicyXML::document() 
{
  QListViewItem * item = listViewItem()->parent();
  MIRO_ASSERT(item != NULL);
  ItemMap::const_iterator i = itemMap_.find(item);
  MIRO_ASSERT(i != itemMap_.end());
  PolicyDocumentXML * document = dynamic_cast<PolicyDocumentXML *>(i->second);
  MIRO_ASSERT(document != NULL);

  return document;
}


void
PolicyXML::rename(QString const& _name)
{
  if (document()->uniqueName(_name)) {
    document()->policyRenaming(name(), _name);
    ItemXML::rename(_name);
  }
  else {
    QMessageBox::warning(NULL, 
			 "Rename Policy", 
			 "Policy " + _name + " allready exists.\n" + 
			 "Policy not renamed.");
  }
}

void
PolicyXML::moveUp()
{
  Item * i = predecessor();
  if (i == NULL)
    return;

  PolicyXML * p = dynamic_cast<PolicyXML * >(i);

  if (p == NULL || !hasSubpolicy(p->name())) {
    Super::moveUp();
  }
  else {
    QMessageBox::warning(NULL, 
			 "Move Policy", 
			 QString("Policy ") + p->name() +
			 " is used as subpolicy by policy " + name() + 
			 ".\nPolicy " + name() + " not moved.");      
  }
}

void
PolicyXML::moveDown()
{
  Item * i = successor();
  if (i == NULL)
    return;

  PolicyXML * p = dynamic_cast<PolicyXML * >(i);

  if (p == NULL || !p->hasSubpolicy(name()))
    Super::moveDown();
  else {
    QMessageBox::warning(NULL, 
			 "Move Policy", 
			 QString("Policy ") + name() + 
			 " is used as subpolicy by policy " + p->name() + 
			 ".\nPolicy " + name() + " not moved.");

  }
}

void
PolicyXML::addActionPattern()
{
  bool ok = false;
  QString text = QInputDialog::getText(
                    tr( "Add Action Pattern" ),
                    tr( "Pattern name:" ),
                    QLineEdit::Normal, QString::null, &ok);
  if ( ok && !text.isEmpty() ) {
    if (!hasPattern(text)) {
      QDomElement e = 
	node_.ownerDocument().createElement(ActionPatternXML::XML_TAG);
      e.setAttribute(XML_ATTRIBUTE_KEY, text);
      QDomNode n = node_.insertBefore(e, QDomNode());
      MIRO_ASSERT(!n.isNull());

      PatternXML * pattern = new ActionPatternXML(n, 
						  listViewItem(), NULL,
						  this, text);
      if (widget_) {
	PolicyView * view = dynamic_cast<PolicyView *>(widget_);
	MIRO_ASSERT(view != NULL);
	view->addPatternWidget(pattern);
      }
    }
    else
      QMessageBox::warning(NULL, 
			   "Add Action Pattern", 
			   "Pattern " + text + " allready exists.\n" + 
			   "No action pattern added.");

  }
  setModified();

}

void
PolicyXML::addSubpolicy(int _id)
{
  bool ok = false;
  QString text = QInputDialog::getText(
                    tr( "Add Subpolicy" ),
                    tr( "Pattern name:" ),
                    QLineEdit::Normal, QString::null, &ok);
  if ( ok && !text.isEmpty() ) {
    if (!hasPattern(text)) {
      QDomElement e = 
	node_.ownerDocument().createElement(SubpolicyXML::XML_TAG);
      e.setAttribute(XML_ATTRIBUTE_KEY, text);
      e.setAttribute(SubpolicyXML::XML_ATTRIBUTE_TYPE, 
		     menuAddSubpolicy_->text(_id));
      QDomNode n = node_.insertBefore(e, QDomNode());
      MIRO_ASSERT(!n.isNull());

      PatternXML * pattern = new SubpolicyXML(n, 
					      listViewItem(), NULL,
					      this, text);

      if (widget_) {
	PolicyView * view = dynamic_cast<PolicyView *>(widget_);
	MIRO_ASSERT(view != NULL);
	view->addPatternWidget(pattern);
      }
      setModified();
    }
    else
      QMessageBox::warning(NULL, 
			   "Add Subpolicy", 
			   "Pattern " + text + " allready exists.\n" + 
			   "No subpolicy added.");
  }
}

void
PolicyXML::addDownTransition(int _id)
{
  QStringVector v = unboundDownTransitionMessages();
  QStringList l;
  QStringVector::const_iterator first, last = v.end();
  for (first = v.begin(); first != last; ++first) {
    l.push_back(*first);
  }

  bool ok = false;
  QString msg = QInputDialog::getItem(
                    tr( "Add downward Transition" ),
                    tr( "Transition message:" ),
                    l, 0, false, &ok);
  if ( ok && !msg.isEmpty() ) {
    if (!hasTransition(msg)) {
      addDownTransition(msg, menuAddDownTransition_->text(_id));
    }
    else
      QMessageBox::warning(NULL, 
			   "Add downward Transition", 
			   "Transition message " + msg + " allready bound.\n" + 
			   "No transition added.");
  }
}


bool
PolicyXML::deletable()
{
  QListViewItem * i = listViewItem()->nextSibling();
  
  while (i != NULL) {
    PolicyXML * p = dynamic_cast<PolicyXML *>(itemFromListViewItem(i));
    if (p != NULL &&
	p->hasSubpolicy(name())) {
      QMessageBox::warning(NULL, 
			   "Delete Policy", 
			   QString("Policy ") + name() + 
			   " is used as subpolicy by policy " + p->name() + 
			   ".\nPolicy " + name() + " can not be deleted.");
      return false;
    }

    i = i->nextSibling();
  }

  return Super::deletable();
}

void 
PolicyXML::buildSubtree() 
{
  QDomNode n = node_.firstChild();
  QListViewItem * pre = NULL;
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull()) {
      ItemXML * param = NULL;
      try {
	if (e.tagName() == ActionPatternXML::XML_TAG &&
	    e.hasAttribute(XML_ATTRIBUTE_KEY)) {
	  param =
	    new ActionPatternXML(e,
				 listViewItem(), pre,
				 this, e.attribute(XML_ATTRIBUTE_KEY));
	}
	else if (e.tagName() == SubpolicyXML::XML_TAG &&
	    e.hasAttribute(XML_ATTRIBUTE_KEY)) {
	  param =
	    new SubpolicyXML(e, 
			     listViewItem(), pre,
			     this, e.attribute(XML_ATTRIBUTE_KEY));
	}
	else if (e.tagName() == DownTransitionXML::XML_TAG &&
	    e.hasAttribute(DownTransitionXML::XML_ATTRIBUTE_MSG)) {
	  param =
	    new DownTransitionXML(e, 
				  listViewItem(), pre,
				  this,
				  e.attribute(DownTransitionXML::XML_ATTRIBUTE_MSG));
	}
	if (param != NULL) {
	  //	  param->init();
	  pre = param->listViewItem();
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

bool
PolicyXML::hasSubpolicy(QString const& _name) const
{
  QDomNode n = node_.firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull() &&
	e.tagName() == SubpolicyXML::XML_TAG &&
	e.attribute(SubpolicyXML::XML_ATTRIBUTE_TYPE) == _name) {
      return true;
    }
    n = n.nextSibling();
  }
  return false;
}

bool
PolicyXML::hasPattern(QString const& _name) const
{
  QDomNode n = node_.firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull() &&
	e.attribute(SubpolicyXML::XML_ATTRIBUTE_KEY) == _name) {
      return true;
    }
    n = n.nextSibling();
  }
  return false;
}

QStringVector 
PolicyXML::transitionMessages() const
{
  QStringVector v;

  QObjectList const * childs = children(); 
  if (childs != NULL) {
    QPtrListIterator<QObject> it(*childs);
    QObject * child;
    while ( (child = it.current()) != 0 ) {
      PatternXML * pattern = dynamic_cast<PatternXML *>(child);
      if (pattern != NULL) {
	QStringVector t = pattern->unboundTransitionMessages();
	v.insert(v.end(), t.begin(), t.end());
      }
      ++it;
    }        
  }

  std::sort(v.begin(), v.end());
  QStringVector::iterator last = std::unique(v.begin(), v.end());
  v.erase(last, v.end());

  return v;
}

QStringVector
PolicyXML::boundTransitionMessages() const
{
  QStringVector v;

  QObjectList const * childs = children(); 
  if (childs != NULL) {
    QPtrListIterator<QObject> it(*childs);
    QObject * child;
    while ( (child = it.current()) != 0 ) {
      DownTransitionXML * t = dynamic_cast<DownTransitionXML *>(child);
      if (t != NULL) {
	v.push_back(t->name());
      }
      ++it;
    }
  }

  std::sort(v.begin(), v.end());

  cout << "downward bound transition messages of " << name() << endl;
  for (unsigned int i = 0; i < v.size(); ++i) {
    cout << "  " << v[i];
  }
  cout << endl;

  return v;
}

QStringVector
PolicyXML::unboundDownTransitionMessages() const
{
  QStringVector v;
  QStringVector messages = transitionMessages();
  QStringVector extMsg = 
    const_cast<PolicyXML *>(this)->document()->externalMessages();
  messages.insert(messages.end(), extMsg.begin(), extMsg.end());
  std::sort(messages.begin(), messages.end());
  QStringVector::iterator last = std::unique(messages.begin(), messages.end());
  messages.erase(last, messages.end());

  QStringVector boundMessages = boundTransitionMessages();

  v.reserve(messages.size());
  std::set_difference(messages.begin(), messages.end(),
		      boundMessages.begin(), boundMessages.end(),
		      std::back_insert_iterator<QStringVector>(v));

  return v;
}

