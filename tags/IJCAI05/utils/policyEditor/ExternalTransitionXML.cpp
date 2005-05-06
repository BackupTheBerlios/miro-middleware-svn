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
