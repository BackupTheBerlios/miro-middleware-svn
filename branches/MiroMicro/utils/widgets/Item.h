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
#ifndef Item_h
#define Item_h

#include <qobject.h>

#include <map>

// forward declarations
class QPopupMenu;
class QListView;
class QListViewItem;

//! This class represents a QListViewItem descendand
/** 
 * As we want a QObject as base class for signals and slots,
 * we need to tie the QListViewItem and the Item class together
 * by the use of a std::map. Sorry, for the inconvenience.
 */
class Item : public QObject
{
  Q_OBJECT;

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! Base class type.
  typedef QObject Super;

public:
  //----------------------------------------------------------------------------
  // public types
  //----------------------------------------------------------------------------

  // Mapping QListViewItem instances to Item instances.
  typedef std::map<QListViewItem *, Item *> ItemMap;

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor, creating a QListView sibling item.
  Item(QListViewItem * _parentItem, QListViewItem * _pre = NULL,
       QObject * _parent = NULL, const char * _name = NULL);
  //! Initializing constructor, creating a QListView toplevel item.
  Item(QListView * _view, QListViewItem * _pre = NULL,
       QObject * _parent = NULL, const char * _name = NULL);
  //! Virtual destructor.
  virtual ~Item();

  //! Accessor for the associated list view.
  QListViewItem * listViewItem();
  //! Const accessor for the associated list view.
  QListViewItem const * listViewItem() const;

  //! Move item up in list view.
  virtual void moveUp();
  //! Move item down in list view.
  virtual void moveDown();
  //! Update the item tree.
  virtual void update();
  //! Populate the provided context menu with entries for the item.
  virtual void contextMenu(QPopupMenu& _menu);

  //----------------------------------------------------------------------------
  // static public methods
  //----------------------------------------------------------------------------

  //! Accesor for the QListViewItem/Item map.
  static ItemMap const& itemMap();
  //! Retriev the associated Item for a QListView.
  static Item * itemFromListViewItem(QListViewItem * _lvi);

public slots:
  //----------------------------------------------------------------------------
  // public sots
  //----------------------------------------------------------------------------

  //! Move the element before the predecessor element.
  void up();
  //! Move the element behind the successor element.
  void down();

protected:
  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------

  //! Get the pedecessor item in the QListView.
  Item * predecessor();
  //! Get the successor item in the QListView.
  Item * successor();

  //----------------------------------------------------------------------------
  // protected members
  //----------------------------------------------------------------------------

  //! The map instance for QListViewItem/Item association.
  static ItemMap itemMap_;

private:
  //----------------------------------------------------------------------------
  // private members
  //----------------------------------------------------------------------------

  //! Pointer to the corresponding QListViewItem.
  QListViewItem * listViewItem_;

  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  Item(Item const&);
};

inline
QListViewItem *
Item::listViewItem() {
  return listViewItem_;
}
inline
QListViewItem const *
Item::listViewItem() const {
  return listViewItem_;
}
inline
Item::ItemMap const&
Item::itemMap() {
  return itemMap_;
}

#endif
