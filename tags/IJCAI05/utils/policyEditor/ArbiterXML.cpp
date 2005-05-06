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

#include "ArbiterXML.h"

#include <qlistview.h>
#include <qpopupmenu.h>

QString const ArbiterXML::XML_TAG("arbiter");

ArbiterXML::ArbiterXML(QDomNode& _node,
		       QListViewItem * _parentItem, QListViewItem * _pre,
		       QObject * _parent, const char * _name) :
  Super(type(_node, XML_ATTRIBUTE_KEY),
	_node, _parentItem, _pre, _parent, _name)
{
  listViewItem()->setText(2, "Arbiter");
}

void
ArbiterXML::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Set Parameters", this, SLOT(slotSetParameters()));
}

