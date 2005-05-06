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
