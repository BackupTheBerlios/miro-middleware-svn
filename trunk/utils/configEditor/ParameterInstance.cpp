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

#include "ParameterInstance.h"

#include "miro/Log.h"

#include <qpopupmenu.h>

QString const ParameterInstance::XML_TAG = "instance";
QString const ParameterInstance::XML_ATTRIBUTE_TYPE = "type";

ParameterInstance::ParameterInstance(QListViewItem * _listViewItem, 
				     QListViewItem * _pre,
				     QDomNode const& _node,
				     QObject * _parent, const char * _name) :
  Super(type(_node, XML_ATTRIBUTE_TYPE),
	_node,
	_listViewItem, _pre, _parent, _name)
{
}

void
ParameterInstance::moveUp()
{
  QDomElement element = node_.toElement();
  MIRO_ASSERT(!element.isNull());
  QDomNode pre = node_.previousSibling();
  while (!pre.isNull()) {
    QDomElement e = pre.toElement();
    if (!e.isNull() &&
	e.tagName() == element.tagName()) {
      break;
    }
    pre = pre.previousSibling();
  }

  if (!pre.isNull()) {
    Super::moveUp();
    node_.parentNode().insertBefore(node_, pre);
    setModified();
  }
}

void
ParameterInstance::moveDown()
{
  QDomElement element = node_.toElement();
  MIRO_ASSERT(!element.isNull());

  QDomNode succ = node_.nextSibling();
  while (!succ.isNull()) {
    QDomElement e = succ.toElement();
    if (!e.isNull() &&
	e.tagName() == element.tagName()) {
      break;
    }
    succ = succ.nextSibling();
  }

  if (!succ.isNull()) {
    Super::moveDown();
    node_.parentNode().insertAfter(node_, succ);
    setModified();
  }
}

void
ParameterInstance::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Set Name", this, SLOT(slotRename()));
  _menu.insertItem("Set Parameters", this, SLOT(slotSetParameters()));
  _menu.insertSeparator();
  _menu.insertItem("Up", this, SLOT(up()));
  _menu.insertItem("Down", this, SLOT(down()));
  _menu.insertSeparator();
  _menu.insertItem("Delete", this, SLOT(slotDelete()));
}

