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

#include "TransitionXML.h"
#include "PatternXML.h"
#include "PolicyXML.h"
#include "PatternWidget.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qinputdialog.h>
#include <qmessagebox.h>

QString const TransitionXML::XML_TAG("transition");
QString const TransitionXML::XML_ATTRIBUTE_MSG("message");
QString const TransitionXML::XML_ATTRIBUTE_TARGET("target");

TransitionXML::TransitionXML(QDomNode& _node,
			     QListViewItem * _parentItem, QListViewItem * _pre,
			     QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name)
{
  listViewItem()->setText(1, target());
  listViewItem()->setText(2, "Transition");
}

PatternXML *
TransitionXML::pattern() const
{
  Item * item = itemFromListViewItem(listViewItem()->parent());
  PatternXML * p = dynamic_cast<PatternXML *>(item);
  MIRO_ASSERT(p != NULL);

  return p;
}

QString
TransitionXML::message()
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());
  MIRO_ASSERT(e.hasAttribute(XML_ATTRIBUTE_MSG));

  return e.attribute(XML_ATTRIBUTE_MSG);
}

QString
TransitionXML::target() 
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());
  MIRO_ASSERT(e.hasAttribute(XML_ATTRIBUTE_TARGET));

  return e.attribute(XML_ATTRIBUTE_TARGET);
}

void
TransitionXML::setTarget(QString const& _target) 
{
  QDomElement e = node_.toElement();
  MIRO_ASSERT(!e.isNull());

  e.setAttribute(XML_ATTRIBUTE_TARGET, _target);
  listViewItem()->setText(1, _target);
  setModified();
  updateView();
}

void
TransitionXML::contextMenu(QPopupMenu& _menu)
{
    // build add transition submenu
  menuSetTarget_ = new QPopupMenu(&_menu);

  QStringList targets = pattern()->policy()->patternList();
  
  QStringList::const_iterator first, last = targets.end();
  for (first = targets.begin(); first != last; ++first)
    menuSetTarget_->insertItem(*first);

  connect(menuSetTarget_, SIGNAL(activated(int)), this, SLOT(setTarget(int)));

  _menu.insertItem("Set Message", this, SLOT(setMessage()));
  _menu.insertItem("Set Target", menuSetTarget_);
  _menu.insertSeparator();
  _menu.insertItem("Delete", this, SLOT(slotDelete()));

}

void
TransitionXML::setMessage()
{
  bool ok = false;
  QStringVector v = pattern()->unboundTransitionMessages();
  QStringList l;
  QStringVector::const_iterator first, last = v.end();
  for (first = v.begin(); first != last; ++first) {
    l.push_back(*first);
  }

  QString msg = QInputDialog::getItem(
                    tr( "Set transition message" ),
                    tr( "Transition message:" ),
                    l, 0, false, &ok);
  if ( ok && !msg.isEmpty() ) {
    if (!pattern()->hasTransition(msg)) {
      QDomElement e = node_.toElement();
      e.setAttribute(XML_ATTRIBUTE_MSG, msg);
      listViewItem()->setText(0, msg);
      setModified();
      updateView();
    }
    else
      QMessageBox::warning(NULL, 
			   "Set transition message", 
			   "Transition message " + msg + " allready bound.\n" + 
			   "Transition message unchanged.");
  }
}

void
TransitionXML::setTarget(int _id)
{
  if (menuSetTarget_->text(_id) != target()) {
    setTarget(menuSetTarget_->text(_id));
  }
}

void
TransitionXML::updateView()
{
  PatternXML * p = pattern();
  if (p->widget()) {
    PatternWidget * w = dynamic_cast<PatternWidget *>(p->widget());
    MIRO_ASSERT(w != NULL);
    w->parentWidget()->update();
  }
}
