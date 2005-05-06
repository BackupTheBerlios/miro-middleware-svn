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

#include "ParameterSingleton.h"

#include "miro/Log.h"

ParameterSingleton::ParameterSingleton(QListViewItem * _listViewItem, 
				       QListViewItem * _pre,
				       QDomNode const& _node,
				       QObject * _parent, const char * _name) :
  Super(type(_node, XML_ATTRIBUTE_KEY),
	_node, 
	_listViewItem, _pre, _parent, _name)
{
}

void
ParameterSingleton::moveUp()
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
ParameterSingleton::moveDown()
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

