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

#include "PatternXML.h"
#include "TransitionXML.h"
#include "BehaviourXML.h"
#include "ActionPatternXML.h"
#include "SubpolicyXML.h"
#include "PolicyXML.h"
#include "PolicyView.h"
#include "PatternWidget.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qinputdialog.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qimage.h>

#include <algorithm>
#include <iostream>

namespace {
  // this is a KDE icon
  // (c) by KDE
  unsigned char const pixStart[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0x61, 0x00, 0x00, 0x00,
    0xb4, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9c, 0xa5, 0x93, 0x6b, 0x0e, 0xc3,
    0x20, 0x0c, 0x83, 0x1d, 0x4e, 0x6e, 0x6e, 0x96, 0x9e, 0xcc, 0xfb, 0xc1,
    0xd2, 0x02, 0x09, 0x95, 0xa6, 0x59, 0xaa, 0x90, 0xd2, 0x3c, 0x3e, 0xb7,
    0xc1, 0x24, 0xa1, 0x92, 0x99, 0x2d, 0x2f, 0x24, 0x59, 0x95, 0xd7, 0xca,
    0xea, 0x1f, 0x54, 0x36, 0x30, 0x33, 0x05, 0x59, 0x9c, 0xbd, 0xf7, 0x12,
    0xd5, 0x2a, 0x0b, 0x81, 0x2f, 0x01, 0x36, 0x81, 0x57, 0x36, 0x8e, 0x16,
    0xdc, 0xd7, 0xf3, 0xa4, 0x23, 0x01, 0xc9, 0x25, 0x46, 0xb2, 0x24, 0x38,
    0x36, 0xd8, 0xe3, 0xf6, 0xf5, 0xb2, 0x37, 0x49, 0x16, 0xaa, 0xe9, 0x41,
    0x00, 0xe4, 0x8f, 0x99, 0x08, 0xaa, 0xe9, 0x6f, 0x14, 0x6d, 0x4b, 0x28,
    0xa7, 0xbf, 0xe9, 0xef, 0x45, 0x82, 0xa4, 0x58, 0x16, 0x49, 0x90, 0x3b,
    0x44, 0x52, 0xbb, 0x48, 0x0a, 0xc0, 0xfd, 0x44, 0x5d, 0x0b, 0x74, 0x09,
    0xb8, 0xae, 0xd1, 0x94, 0x24, 0x7a, 0xef, 0x69, 0x98, 0xfb, 0x58, 0x2e,
    0xf7, 0x67, 0xd9, 0x6e, 0x0b, 0x51, 0x1c, 0x89, 0xb3, 0x46, 0x33, 0xa6,
    0xbc, 0x64, 0xc1, 0x3d, 0x2c, 0x60, 0xc1, 0x05, 0x46, 0xcc, 0x3d, 0x5b,
    0x58, 0x7e, 0xe3, 0x7c, 0x85, 0xf7, 0x85, 0x79, 0xee, 0xc7, 0x1a, 0xff,
    0x00, 0x08, 0x18, 0xab, 0x57, 0x6c, 0x2e, 0x2b, 0x3f, 0x00, 0x00, 0x00,
    0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
  };

  QPixmap * pmStartPattern = NULL;
  QPixmap * pmNull = NULL;
}


QString const PatternXML::XML_ATTRIBUTE_START("start");
QString const PatternXML::XML_ATTRIBUTE_X("x");
QString const PatternXML::XML_ATTRIBUTE_Y("y");

PatternXML::PatternXML(QDomNode const& _node,
		       QListViewItem * _parentItem, QListViewItem * _pre,
		   QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name),
  menuAddTransition_(NULL)
{
  if (pmStartPattern == NULL) {
    QImage img;
    img.loadFromData(pixStart, sizeof(pixStart), "PNG");
    pmStartPattern = new QPixmap( img );
    pmNull = new QPixmap();
  }

  if (startPattern()) {
    listViewItem()->setPixmap(0, *pmStartPattern);
  }
}

PolicyXML *
PatternXML::policy()
{
  PolicyXML * p = dynamic_cast<PolicyXML *>(this);
  if (p == NULL) {
    Item * item = itemFromListViewItem(listViewItem()->parent());
    p = dynamic_cast<PolicyXML *>(item);
    MIRO_ASSERT(p != NULL);
  }

  return p;
}

bool
PatternXML::startPattern()
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());

  return e.attribute(XML_ATTRIBUTE_START) == "true";
}

void
PatternXML::startPattern(bool _flag)
{
  if (_flag == startPattern())
    return;

  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());
  
  if (_flag) {
    e.setAttribute(XML_ATTRIBUTE_START, "true");
    listViewItem()->setPixmap(0, *pmStartPattern);
  }
  else {
    e.setAttribute(XML_ATTRIBUTE_START, "false");
    listViewItem()->setPixmap(0, *pmNull);
  }

  setModified();
  if (widget_)
    widget_->update();
}

bool
PatternXML::hasTransition(QString const& _msg) const
{
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    TransitionXML * transition = dynamic_cast<TransitionXML *>(item);
    if (transition != NULL &&
	transition->name() == _msg) {
      return true;
    }
    i = i->nextSibling();
  }
  return false;
}

void
PatternXML::removeTransitionTo(QString const& _target) 
{
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    TransitionXML * t = dynamic_cast<TransitionXML *>(item);
    if (t != NULL &&
	t->target() == _target) {
      t->deleteLater();
      setModified();
      if (widget_) {
	MIRO_ASSERT(widget_->parent() != NULL);
	widget_->parentWidget()->update();
      }
    }
    i = i->nextSibling();
  }
}

void
PatternXML::renameTransitionTo(QString const& _oldTarget, QString const& _newTarget) 
{
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    TransitionXML * t = dynamic_cast<TransitionXML *>(item);
    if (t != NULL &&
	t->target() == _oldTarget) {
      t->setTarget(_newTarget);
      setModified();
      if (widget_) {
	MIRO_ASSERT(widget_->parent() != NULL);
	widget_->parentWidget()->update();
      }
    }
    i = i->nextSibling();
  }
}

TransitionVector
PatternXML::transitions()
{
  TransitionVector rc;

  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    TransitionXML * t = dynamic_cast<TransitionXML *>(item);
    if (t != NULL) {
      rc.push_back(Transition(t->message(), 
			      policy()->patternFromName(t->target())));
    }
    i = i->nextSibling();
  }

  return rc;
}

int
PatternXML::x() 
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());
  return e.attribute(XML_ATTRIBUTE_X, "0").toInt();
}

void
PatternXML::setX(int _x) 
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());
  e.setAttribute(XML_ATTRIBUTE_X, _x);
  setModified();
}

int
PatternXML::y() 
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());
  return e.attribute(XML_ATTRIBUTE_Y, "0").toInt();
}

void
PatternXML::setY(int _y) 
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());
  e.setAttribute(XML_ATTRIBUTE_Y, _y);
  setModified();
}

void
PatternXML::contextMenu(QPopupMenu& _menu)
{
  // build add transition submenu
  menuAddTransition_ = new QPopupMenu(&_menu);

  QStringList targets = policy()->patternList();
  
  QStringList::const_iterator first, last = targets.end();
  for (first = targets.begin(); first != last; ++first)
    menuAddTransition_->insertItem(*first);

  connect(menuAddTransition_, SIGNAL(activated(int)), this, SLOT(addTransition(int)));

  _menu.insertItem("Add Transition", menuAddTransition_);
  _menu.insertSeparator();
  _menu.insertItem("Set Name", this, SLOT(slotRename()));
  _menu.insertItem("Set start Pattern", this, SLOT(setStartPattern()));
  _menu.insertItem("Up", this, SLOT(up()));
  _menu.insertItem("Down", this, SLOT(down()));
  _menu.insertSeparator();
  _menu.insertItem("Delete", this, SLOT(slotDelete()));

//   // debug output
//   QStringVector unbound = unboundTransitionMessages();
//   cout << "unbound transitions:" << endl;
//   for (unsigned int i = 0; i < unbound.size(); ++i) {
//     cout << "  " << unbound[i] << endl;
//   }
//   cout << endl;
}

void
PatternXML::rename(QString const& _name)
{
  if (policy()->uniquePatternName(_name)) {
    policy()->renameTransitionTo(name(), _name);
    Super::rename(_name);
  }
  else {
    QMessageBox::warning(NULL, 
			 "Rename Pattern", 
			 "Pattern " + _name + " allready exists.\n" + 
			 "Pattern not renamed.");
  }
}

void
PatternXML::setStartPattern()
{
  if (!this->startPattern()) {
    QListViewItem * i = listViewItem()->parent()->firstChild();
    while (i != NULL) {
      Item * item = itemFromListViewItem(i);
      if (item != this) {
	PatternXML * pattern = dynamic_cast<PatternXML *>(item);
	pattern->startPattern(false);
      }
      i = i->nextSibling();
    }
    this->startPattern(true);
  }
}

bool
PatternXML::tryAddTransition(QString const& _msg, PatternXML * _target)
{
  if (!hasTransition(_msg)) {
    addTransition(_msg, _target->name());
    return true;
  }
  return false;
}

void
PatternXML::addTransition(QString const& _msg, QString const& _target)
{
  QDomElement e = 
    node_.ownerDocument().createElement(TransitionXML::XML_TAG);
  e.setAttribute(TransitionXML::XML_ATTRIBUTE_MSG, _msg);
  e.setAttribute(TransitionXML::XML_ATTRIBUTE_TARGET,
		 _target);
  QDomNode n = node_.insertBefore(e, QDomNode());
  MIRO_ASSERT(!n.isNull());
      
  new TransitionXML(n, 
		    listViewItem(), NULL,
		    this, _msg);
  setModified();
  if (widget_) {
    MIRO_ASSERT(widget_->parent() != NULL);
    widget_->parentWidget()->update();
  }
}

void
PatternXML::addTransition(int _id)
{
  bool ok = false;

  QStringVector v = unboundTransitionMessages();
  QStringList l;
  QStringVector::const_iterator first, last = v.end();
  for (first = v.begin(); first != last; ++first) {
    l.push_back(*first);
  }

  QString msg = QInputDialog::getItem(
                    tr( "Add Transition" ),
                    tr( "Transition message:" ),
                    l, 0, false, &ok);
  if ( ok && !msg.isEmpty() ) {
    if (!hasTransition(msg)) {
      addTransition(msg, menuAddTransition_->text(_id));
    }
    else
      QMessageBox::warning(NULL, 
			   "Add Transition", 
			   "Transition message " + msg + " allready bound.\n" + 
			   "No transition added.");
  }

}

bool
PatternXML::deletable()
{
  if (startPattern()) {
    QMessageBox::warning(NULL, 
			 "Delete Pattern", 
			 QString("Pattern ") + name() + " is a start pattern\n" + 
			 "Can not delete start pattern.");
    return false;
  }
  else 
    return Super::deletable();
}

void
PatternXML::deleteItem()
{
  policy()->removeTransitionTo(name());
  Super::deleteItem();
}


QStringVector
PatternXML::unboundTransitionMessages()
{
  QStringVector v;
  QStringVector messages = transitionMessages();
  QStringVector boundMessages = boundTransitionMessages();

  v.reserve(messages.size());
  std::set_difference(messages.begin(), messages.end(),
		      boundMessages.begin(), boundMessages.end(),
		      std::back_insert_iterator<QStringVector>(v));

  return v;
}

QStringVector
PatternXML::boundTransitionMessages() const
{
  QStringVector v;
  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    TransitionXML * t = dynamic_cast<TransitionXML *>(item);
    if (t != NULL) {
      v.push_back(t->name());
    }
    i = i->nextSibling();
  }

  std::sort(v.begin(), v.end());

  std::cout << "bound transition messages of " << name() << std::endl;
  for (unsigned int i = 0; i < v.size(); ++i) {
    std::cout << "  " << v[i];
  }
  std::cout << std::endl;

  return v;
}


QStringVector
PatternXML::transitionMessages() const
{
  QStringVector v;

  QListViewItem * i = listViewItem()->firstChild();
  while (i != NULL) {
    Item * item = itemFromListViewItem(i);
    QStringVector v2;
    BehaviourXML * behaviour = dynamic_cast<BehaviourXML *>(item);
    if (behaviour != NULL) {
      v2 = behaviour->transitionMessages();
    }
    else {
      PatternXML * pattern = dynamic_cast<PatternXML *>(item);
      if (pattern != NULL) {
	v2 = pattern->transitionMessages();
      }
    }

    v.insert(v.end(), v2.begin(), v2.end());
    i = i->nextSibling();
  }
  
  std::sort(v.begin(), v.end());
  QStringVector::iterator last = std::unique(v.begin(), v.end());
  v.erase(last, v.end());

  std::cout << "transition messages of " << name() << std::endl;
  for (unsigned int i = 0; i << v.size(); ++i) {
	  std::cout << "  " << v[i];
  }
  std::cout << std::endl;

  return v;
}

void
PatternXML::removeChild(QObject * _obj)
{
  Super::removeChild(_obj);

  if (widget_) {
    PatternWidget * w = dynamic_cast<PatternWidget *>(widget_);
    if (w != NULL) {
      w->init();
    }
    else {
      PolicyView * v = dynamic_cast<PolicyView *>(widget_);
      MIRO_ASSERT(v != NULL);
      v->viewport()->update();
    }
  }
}
