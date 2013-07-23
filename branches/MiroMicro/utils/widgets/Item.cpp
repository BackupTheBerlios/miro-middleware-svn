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
#include "Item.h"

#include "miro/Log.h"

#include <qstring.h>
#include <qobjectlist.h>
#include <qlistview.h>

Item::ItemMap Item::itemMap_;

//----------------------------------------------------------------------------
// public methods
//----------------------------------------------------------------------------

Item::Item(QListViewItem * _parentItem, QListViewItem * _pre,
	   QObject * _parent, const char * _name) :
  Super(_parent, _name),
  listViewItem_(new QListViewItem(_parentItem, _pre))
{
  listViewItem()->setText(0, name());
  itemMap_.insert(std::make_pair(listViewItem_, this));
}

Item::Item(QListView * _view, QListViewItem * _pre,
	   QObject * _parent, const char * _name) :
  Super(_parent, _name),
  listViewItem_(new QListViewItem(_view, _pre))
{
  listViewItem()->setText(0, name());
  itemMap_.insert(std::make_pair(listViewItem_, this));
}

Item::~Item()
{
  //  cout << name() << " deleting children" << endl;

  if (children()) {
    QObjectList childList = *children();
    QObject * c = childList.first();
    while (c) {
      delete c;
      c = childList.next();
    }
  }

  //  cout << name() << " deleting listviewitem" << endl;

  delete listViewItem_;
  itemMap_.erase(listViewItem_);

  //  cout << name() << " deleting" << endl;
}

void
Item::update()
{
}

void 
Item::contextMenu(QPopupMenu&)
{
}

//------------------------------------------------------------------------------
// public slots
//------------------------------------------------------------------------------

void 
Item::up()
{
  moveUp();
}

void
Item::down()
{
  moveDown();
}

void
Item::moveUp()
{
  // find the pre predecessor of the item
  QListViewItem * prePre = NULL;
  QListViewItem * pre = NULL;
  QListViewItem * self = NULL;

  // get the parents first child
  self = (listViewItem()->parent() == NULL)?
    listViewItem()->listView()->firstChild() :
    listViewItem()->parent()->firstChild();

  // find ourselves
  while (self != listViewItem()) {
    MIRO_ASSERT(self != NULL);

    prePre = pre;
    pre = self;
    self = self->nextSibling();
  }

  if (pre != NULL) {
    if (prePre != NULL) {
      listViewItem()->moveItem(prePre);
    }
    else {
      pre->moveItem(listViewItem());
    }
  }
}

void
Item::moveDown()
{
  QListViewItem * succ = listViewItem()->nextSibling();
  if (succ != NULL) {
    listViewItem()->moveItem(succ);
  }
}

Item *
Item::predecessor()
{
  // find the pre predecessor of the item
  QListViewItem * prePre = NULL;
  QListViewItem * pre = NULL;
  QListViewItem * self = NULL;

  // get the parents first child
  self = (listViewItem()->parent() == NULL)?
    listViewItem()->listView()->firstChild() :
    listViewItem()->parent()->firstChild();

  // find ourselves
  while (self != listViewItem()) {
    MIRO_ASSERT(self != NULL);

    prePre = pre;
    pre = self;
    self = self->nextSibling();
  }

  return itemFromListViewItem(pre);
}

Item *
Item::successor()
{
  return itemFromListViewItem(listViewItem_->nextSibling());
}

Item *
Item::itemFromListViewItem(QListViewItem * _lvi)
{
  if (_lvi == NULL)
    return NULL;

  ItemMap::const_iterator i = itemMap_.find(_lvi);
  MIRO_ASSERT(i != itemMap_.end());
  return i->second;
}
