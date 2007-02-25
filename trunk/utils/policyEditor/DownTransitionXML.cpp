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
#include "DownTransitionXML.h"
#include "PolicyXML.h"
#include "PolicyView.h"

#include "miro/Log.h"

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qinputdialog.h>
#include <qmessagebox.h>

QString const DownTransitionXML::XML_TAG("transitionpattern");

DownTransitionXML::DownTransitionXML(QDomNode& _node,
			     QListViewItem * _parentItem, QListViewItem * _pre,
			     QObject * _parent, const char * _name) :
  Super(_node, _parentItem, _pre, _parent, _name)
{
  listViewItem()->setText(1, target());
  listViewItem()->setText(2, "Downward Transition");
}

PolicyXML *
DownTransitionXML::policy() const
{
  Item * item = itemFromListViewItem(listViewItem()->parent());
  PolicyXML * p = dynamic_cast<PolicyXML *>(item);
  MIRO_ASSERT(p != NULL);

  return p;
}

void
DownTransitionXML::contextMenu(QPopupMenu& _menu)
{
    // build add transition submenu
  menuSetTarget_ = new QPopupMenu(&_menu);

  QStringList targets = policy()->patternList();
  
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
DownTransitionXML::updateView()
{
  PatternXML * p = policy();
  if (p->widget()) {
    PolicyView * v = dynamic_cast<PolicyView *>(p->widget());
    MIRO_ASSERT(v != NULL);
    v->viewport()->update();
  }
}
