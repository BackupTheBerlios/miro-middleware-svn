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
#include "ExternalTransitionXML.h"
#include "PolicyDocumentXML.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qinputdialog.h>
#include <qmessagebox.h>

QString const ExternalTransitionXML::XML_TAG("external_transition");
QString const ExternalTransitionXML::XML_ATTRIBUTE_MSG("message");

ExternalTransitionXML::ExternalTransitionXML(QDomNode& _node,
			     QListViewItem * _parentItem, QListViewItem * _pre,
			     QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name)
{
  listViewItem()->setText(2, "ExternalTransition");
}

PolicyDocumentXML *
ExternalTransitionXML::document() const
{
  Item * item = itemFromListViewItem(listViewItem()->parent());
  PolicyDocumentXML * d = dynamic_cast<PolicyDocumentXML *>(item);
  MIRO_ASSERT(d != NULL);

  return d;
}

QString
ExternalTransitionXML::message()
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());
  MIRO_ASSERT(e.hasAttribute(XML_ATTRIBUTE_MSG));

  return e.attribute(XML_ATTRIBUTE_MSG);
}

void
ExternalTransitionXML::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Set Message", this, SLOT(setMessage()));
  _menu.insertSeparator();
  _menu.insertItem("Delete", this, SLOT(slotDelete()));

}

void
ExternalTransitionXML::setMessage()
{
  bool ok = false;
  QString msg = QInputDialog::getText(
                    tr( "Set transition message" ),
                    tr( "External message:" ),
                    QLineEdit::Normal, message(), &ok, NULL );
  if ( ok && msg != message() ) {
    if (!document()->hasExternalMessage(msg)) {
      QDomElement e = node_.toElement();
      e.setAttribute(XML_ATTRIBUTE_MSG, msg);
      listViewItem()->setText(0, msg);
      setModified();
    }
    else
      QMessageBox::warning(NULL, 
			   "Set transition message", 
			   "External transition message " + msg + " allready exists.\n" + 
			   "External transition message unchanged.");
  }
}
