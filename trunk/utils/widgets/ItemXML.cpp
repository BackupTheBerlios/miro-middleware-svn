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

#include "ItemXML.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qobjectlist.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qlistview.h>

QString const ItemXML::XML_ATTRIBUTE_KEY = "name";

ItemXML::ItemXML(QDomNode const& _node,
		 QListViewItem * _parentItem, QListViewItem * _pre,
		 QObject * _parent, const char * _name) :
  Super(_parentItem, _pre, _parent, _name),
  node_(_node),
  widget_(NULL),
  modified_(false)
{
  MIRO_ASSERT(!node_.isNull());
}

ItemXML::ItemXML(QDomNode const& _node,
		 QListView * _view, QListViewItem * _pre,
		 QObject * _parent, const char * _name) :
  Super(_view, _pre, _parent, _name),
  node_(_node),
  widget_(NULL),
  modified_(false)
{
  MIRO_ASSERT(!node_.isNull());
}

ItemXML::~ItemXML()
{
  delete widget_;
}

bool 
ItemXML::modified() const
{
  bool m = modified_;
  
  if (children()) {
    QObjectListIt it(*children());
    while (!m && it.current() != 0 ) {
      ItemXML const * item = dynamic_cast<ItemXML const *>(it.current());
      if (item != NULL)
	m |= item->modified();
      ++it;
    }
  }
  return m;
}

void 
ItemXML::setModified(bool _modified, bool _recurse) 
{
  modified_ = _modified;

  if (_recurse && children()) {
    QObjectListIt it(*children());
    while (it.current() != 0 ) {
      ItemXML * item = dynamic_cast<ItemXML *>(it.current());
      if (item != NULL)
	item->setModified(_modified, _recurse);
      ++it;
    }
  }
}

void 
ItemXML::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Set Name", this, SLOT(slotRename()));
  _menu.insertSeparator();
}

void
ItemXML::slotRename()
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());

  bool ok = false;
  QString text = 
    QInputDialog::getText(tr( "Rename Entry" ),
			  tr( "New name:" ),
			  QLineEdit::Normal,
			  name(), &ok, NULL);
  if (ok && 
      !text.isEmpty() && 
      text != name()) {
    rename(text);
  }
}

void
ItemXML::moveUp()
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
ItemXML::moveDown()
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

bool
ItemXML::deletable()
{
  int rc = QMessageBox::Ok;
  // Ask for confirmation in case of child elements.
  if (!node_.firstChild().isNull()) {
    rc = QMessageBox::warning(NULL, 
			      "Delete Element",
			      "Deleting this element will delete all its child elements, too." ,
			      QMessageBox::Ok, QMessageBox::Cancel);
  }
  return rc == QMessageBox::Ok;
}

void
ItemXML::slotDelete()
{
  if (deletable())
    deleteItem();
}

//----------------------------------------------------------------------------
// public methods
//----------------------------------------------------------------------------

void 
ItemXML::deleteItem()
{
  // schedule for deletion
  // this will also remove the listviewitem
  deleteLater();

  // remove this node at the parent node
  // as this is not taken care of in
  // the dtor of this class
  QDomNode n = node_.parentNode();
  MIRO_ASSERT(!n.isNull());
  n.removeChild(node_);

  // tell the parent, that he is modified
  // as he will not be able to ask us about it anymore
  ItemXML * p = 
    dynamic_cast<ItemXML *>(parent());
  if (p != NULL) {
    p->setModified();
  }
}

void 
ItemXML::rename(QString const& _name)
{
  // convert to element
  QDomElement element = node_.toElement();
  MIRO_ASSERT(!element.isNull());

  // test for identity
  QString name = element.attribute(XML_ATTRIBUTE_KEY);
  if (name != _name) {
    // rename element
    element.setAttribute(XML_ATTRIBUTE_KEY, _name);
    setName(_name.latin1());
    listViewItem()->setText(0, _name);
    setModified();
  }

  if (widget_)
    widget_->update();
}

void
ItemXML::moveTo(QDomNode& _parent, QDomNode& _predecessor)
{
  // the corresponding syntax in DOM looks slightly different:
  _parent.insertAfter(node_, _predecessor);
  setModified();
}

