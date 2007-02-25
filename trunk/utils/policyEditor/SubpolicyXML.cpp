// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "SubpolicyXML.h"
#include "PolicyDocumentXML.h"
#include "PolicyXML.h"
#include "TransitionXML.h"

#include "miro/Log.h"

#include <qlistview.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>

#include <iostream>
using namespace std;

QString const SubpolicyXML::XML_TAG("subpolicy");
QString const SubpolicyXML::XML_ATTRIBUTE_TYPE("type");

SubpolicyXML::SubpolicyXML(QDomNode& _node,
			   QListViewItem * _parentItem, QListViewItem * _pre,
			   QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name)
{
  listViewItem()->setText(1, node_.toElement().attribute(XML_ATTRIBUTE_TYPE));
  listViewItem()->setText(2, "Subpolicy");

  buildSubtree();
}

void 
SubpolicyXML::policyRenaming(QString const& _oldName, QString const& _newName)
{
  if (node_.toElement().attribute(XML_ATTRIBUTE_TYPE) == _oldName)
    retype(_newName);
}

void
SubpolicyXML::retype(QString const& _newType)
{
  node_.toElement().setAttribute(XML_ATTRIBUTE_TYPE, _newType);
  listViewItem()->setText(1, _newType);
}

QString
SubpolicyXML::type() const
{
  return const_cast<SubpolicyXML *>(this)->
    node_.toElement().attribute(XML_ATTRIBUTE_TYPE);
}

void 
SubpolicyXML::buildSubtree() 
{
  QDomNode n = node_.firstChild();
  QListViewItem * pre = NULL;
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull()) {
      Item * item = NULL;
      try {
	if (e.tagName() == TransitionXML::XML_TAG &&
	    e.hasAttribute(TransitionXML::XML_ATTRIBUTE_MSG)) {
	  item =
	    new TransitionXML(e,
			      listViewItem(), pre,
			      this, e.attribute(TransitionXML::XML_ATTRIBUTE_MSG));
	}
	pre = item->listViewItem();
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

QStringVector
SubpolicyXML::transitionMessages() const
{
  cout << "subpolicy "  << name() << endl;

  PolicyXML * p = const_cast<SubpolicyXML *>(this)->policy();
  MIRO_ASSERT(p != NULL);
  PolicyDocumentXML * d = p->document();
  MIRO_ASSERT(d != NULL);
  PolicyXML * t = d->policyByName(type());
  MIRO_ASSERT(t != NULL);
  return t->unboundTransitionMessages();
}
