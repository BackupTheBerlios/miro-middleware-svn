// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "Item.h"

#include <qlistview.h>
#include <qobjectlist.h>

Item::ItemMap Item::itemMap_;

Item::Item(QObject * _parent, char const * _name) :
  Super(_parent, _name),
  listViewItem_(NULL)
{}

Item::~Item() 
{
  if (children()) {
    QObjectList childList = *children();
    QObject * c = childList.first();
    while (c) {
      delete c;
      c = childList.next();
    }
  }

  itemMap_.erase(listViewItem_);
  delete listViewItem_;
}

void
Item::update()
{
}

void
Item::setListViewItem(QListViewItem * _listViewItem)
{
  if (listViewItem_ != NULL)
    itemMap_.erase(listViewItem_);
  listViewItem_ = _listViewItem;
  itemMap_.insert(std::make_pair(listViewItem_, this));
}

void
Item::contextMenu(QPopupMenu&)
{
}
