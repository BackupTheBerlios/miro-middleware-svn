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
#ifndef notifyObserver_Item
#define notifyObserver_Item

#include <qobject.h>

#include <map>

class QListViewItem;
class QPopupMenu;

class Item : public QObject
{
  Q_OBJECT

  typedef QObject Super;

 public:
  typedef std::map<QListViewItem *, Item*> ItemMap;

  Item(QObject * _parent, char const * _name);
  ~Item();

  virtual void update();
  virtual void contextMenu(QPopupMenu& _menu);
  
  QListViewItem * listViewItem();
  void setListViewItem(QListViewItem * _listViewItem);

  static ItemMap const& itemMap();

protected:
  static ItemMap itemMap_;

private:
  //! List view for displaying the tree.
  QListViewItem * listViewItem_;
};

inline
QListViewItem *
Item::listViewItem() {
  return listViewItem_;
}

inline
Item::ItemMap const&
Item::itemMap() {
  return itemMap_;
}

#endif //notifyObserver_Item
