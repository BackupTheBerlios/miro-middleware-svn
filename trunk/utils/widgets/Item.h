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
#ifndef Item_h
#define Item_h

#include <qobject.h>

#include <map>

// forward declarations
class QPopupMenu;
class QListView;
class QListViewItem;

//! This class represents a BAP item in the XML tree.
/**
 * It encapsulates the xml processing of the item.
 */
class Item : public QObject
{
  Q_OBJECT;

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  typedef QObject Super;

public:
  //----------------------------------------------------------------------------
  // public types
  //----------------------------------------------------------------------------
  typedef std::map<QListViewItem *, Item *> ItemMap;

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  Item(QListViewItem * _parentItem, QListViewItem * _pre = NULL,
       QObject * _parent = NULL, const char * _name = NULL);
  Item(QListView * _view, QListViewItem * _pre = NULL,
       QObject * _parent = NULL, const char * _name = NULL);
  virtual ~Item();

  QListViewItem * listViewItem();
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

  static ItemMap const& itemMap();
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
  Item * predecessor();
  Item * successor();

  //----------------------------------------------------------------------------
  // protected members
  //----------------------------------------------------------------------------
  static ItemMap itemMap_;

private:
  //----------------------------------------------------------------------------
  // private members
  //----------------------------------------------------------------------------
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
