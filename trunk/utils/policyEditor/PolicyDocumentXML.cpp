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
#define QT_ALTERNATE_QTSMANIP

#include "PolicyDocumentXML.h"
#include "PolicyXML.h"
#include "ActionPatternXML.h"
#include "ExternalTransitionXML.h"

#include "utils/widgets/ConfigFile.h"

#include "params/Generator.h"

#include "miro/Log.h"
#include "miro/Exception.h"

#include <qlistview.h>
#include <qobjectlist.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qinputdialog.h>
#include <qmessagebox.h>

#include <iostream>
#include <algorithm>

QString const PolicyDocumentXML::XML_DOCTYPE("MiroPolicyDocument");
QString const PolicyDocumentXML::XML_TAG("structuredpolicy");
QString const PolicyDocumentXML::XML_TAG_FLATPOLICY("policy");

PolicyDocumentXML::PolicyDocumentXML(QDomDocument const& _document, 
				     QListView * _listView,
				     QObject * _parent, const char * _name) :
  Super(_document, _listView, _parent, _name)
{
}

PolicyDocumentXML::~PolicyDocumentXML() 
{}

void 
PolicyDocumentXML::policyRenaming(QString const& _oldName, QString const& _newName)
{
  if (!structuredPolicy())
    return;

  QListViewItem * item = listViewItem()->firstChild();
  while (item != NULL) {
    Item * i = itemFromListViewItem(item);
    PolicyXML * p = dynamic_cast<PolicyXML *>(i);

    if (p != NULL)
      p->policyRenaming(_oldName, _newName);

    item = item->nextSibling();
  }
}

QStringVector
PolicyDocumentXML::policies(QString const& _end) const
{
  QStringVector v;

  QObjectList const * childs = children(); 
  if (childs != NULL) {
    QPtrListIterator<QObject> it(*childs);
    QObject * child;
    while ( (child = it.current()) != 0 ) {
      PolicyXML * policy = dynamic_cast<PolicyXML *>(child);
      if (policy != NULL) {
	if (policy->name() == _end)
	  break;
	v.push_back(policy->name());
      }
      ++it;
    }
    std::sort(v.begin(), v.end());
  }

  return v;
}

QStringVector
PolicyDocumentXML::externalMessages() const
{
  QStringVector v;

  QObjectList const * childs = children(); 
  if (childs != NULL) {
    QPtrListIterator<QObject> it(*childs);
    QObject * child;
    while ( (child = it.current()) != 0 ) {
      ExternalTransitionXML * transition = 
	dynamic_cast<ExternalTransitionXML *>(child);
      if (transition != NULL) {
	v.push_back(transition->message());
      }
      ++it;
    }
        
    std::sort(v.begin(), v.end());
  }

  return v;
}

bool
PolicyDocumentXML::hasExternalMessage(QString const& _msg) const
{
  QObjectList const * childs = children(); 
  if (childs != NULL) {
    QPtrListIterator<QObject> it(*childs);
    QObject * child;
    while ( (child = it.current()) != 0 ) {
      ExternalTransitionXML * transition = 
	dynamic_cast<ExternalTransitionXML *>(child);
      if (transition != NULL &&
	  transition->message() == _msg) {
	return true;
      }
      ++it;
    }
  }

  return false;
}

PolicyXML *
PolicyDocumentXML::policyByName(QString const& _name)
{
  QObjectList const * childs = children(); 
  if (childs != NULL) {
    QPtrListIterator<QObject> it(*childs);
    QObject * child;
    while ((child = it.current()) != 0) {
      PolicyXML * policy = dynamic_cast<PolicyXML *>(child);
      if (policy != NULL &&
	  policy->name() == _name) {
	return policy;
      }
      ++it;
    }
  }

  return NULL;
}

//----------------------------------------------------------------------------  
// inherited public methods

void
PolicyDocumentXML::contextMenu(QPopupMenu& _menu)
{
  if (structuredPolicy())
    _menu.insertItem("Add Policy", this, SLOT(slotAddPolicy()));
    _menu.insertItem("Add External Message", this, SLOT(slotAddExternalMessage()));
}

//----------------------------------------------------------------------------  
// protected methods

bool
PolicyDocumentXML::uniqueName(QString const& _policyName) const
{
  MIRO_ASSERT(!document_.documentElement().isNull());

  QDomNode n = document_.documentElement().firstChild();
  while (!n.isNull()) {
    QDomElement e = n.toElement();
    if (!e.isNull()) {
      if (e.tagName() == PolicyXML::XML_TAG &&
	  e.attribute(XML_ATTRIBUTE_KEY) == _policyName)
	return false;
    }
    n = n.nextSibling();
  }
  return true;
}

bool
PolicyDocumentXML::structuredPolicy() const
{
  MIRO_ASSERT(!document_.documentElement().isNull());

  return document_.documentElement().tagName() == XML_TAG;
}

//----------------------------------------------------------------------------  
// public slots

void 
PolicyDocumentXML::slotAddPolicy()
{
  bool ok = false;
  QString text = QInputDialog::getText(
                    tr( "Add Policy" ),
                    tr( "Policy name:" ),
                    QLineEdit::Normal, QString::null, &ok);
  if ( ok && !text.isEmpty() ) {
    if (uniqueName(text)) {
      QDomElement e = document_.createElement(PolicyXML::XML_TAG);
      e.setAttribute(XML_ATTRIBUTE_KEY, text);
      QDomNode n = document_.documentElement().insertBefore(e, QDomNode());
      MIRO_ASSERT(!n.isNull());

      new PolicyXML(n, 
		    listViewItem(), NULL,
		    this, text);
      setModified();
    }
    else
      QMessageBox::warning(NULL, 
			   "Add Policy", 
			   "Policy " + text + " allready exists.\n" + 
			   "No policy added.");

  }
}

void
PolicyDocumentXML::slotAddExternalMessage()
{
  bool ok = false;
  QString msg = QInputDialog::getText(
                    tr( "Add external transition message" ),
                    tr( "External message:" ),
                    QLineEdit::Normal, "", &ok, NULL );
  if ( ok && !msg.isEmpty()) {
    if (!hasExternalMessage(msg)) {
      QDomElement e = document_.createElement(ExternalTransitionXML::XML_TAG);
      e.setAttribute(ExternalTransitionXML::XML_ATTRIBUTE_MSG, msg);
      QDomNode n = document_.documentElement().insertBefore(e, QDomNode());
      MIRO_ASSERT(!n.isNull());
      
      new ExternalTransitionXML(n,
				listViewItem(), NULL,
				this, msg);
      setModified();
    }
    else
      QMessageBox::warning(NULL, 
			   "Add external transition message", 
			   "External transition message " + msg + " allready exists.\n");
  }
}
//------------------------------------------------------------------------------
// protected methods

void
PolicyDocumentXML::parse()
{
  QDomElement e = document_.documentElement();
  if (e.isNull()) 
    throw Miro::Exception("Empty policy document.");
  
  if (e.tagName() != XML_TAG_FLATPOLICY &&
      e.tagName() != XML_TAG) {
    throw Miro::Exception("No policy document.");
  }

  listViewItem()->setText(0, e.tagName());
  listViewItem()->setText(2, document_.doctype().name());
  listViewItem()->setOpen(true);

  if (structuredPolicy()) {
    
    QDomNode n1 = e.firstChild();
    QListViewItem * pre = NULL;
    while (!n1.isNull()) {
      QDomElement e = n1.toElement();
      ItemXML * item = NULL;
      if (!e.isNull()) {
	if (e.tagName() == PolicyXML::XML_TAG) {
	  item =
	    new PolicyXML(e, 
			  listViewItem(), pre,
			  this, e.attribute(XML_ATTRIBUTE_KEY));
	}
	else if (e.tagName() == ExternalTransitionXML::XML_TAG) {
	  item =
	    new ExternalTransitionXML(e,
				      listViewItem(), pre,
				      this, e.attribute(ExternalTransitionXML::XML_ATTRIBUTE_MSG));
	}
	else {
	  throw Miro::Exception(std::string("Illigal tag: ") + e.tagName().latin1());
	}
	pre = item->listViewItem();
      }
      n1 = n1.nextSibling();
    }
  }
  else {
    new PolicyXML(e, 
		  listViewItem(), NULL,
		  this, e.attribute(XML_ATTRIBUTE_KEY));
  }
}

