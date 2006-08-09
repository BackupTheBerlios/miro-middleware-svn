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
#ifndef EventView_h
#define EventView_h

#include <ace/Time_Value.h>

#include <qdialog.h>

// forward declarations
class QListView;
class QListViewItem;
class FileSet;
namespace CosNotification {
  class StructuredEvent;
}

class EventView : public QWidget
{
  Q_OBJECT

  typedef QWidget Super;

public:
  EventView(FileSet * _fileSet, unsigned int _history, char const * _name);
  virtual ~EventView();

  void setHistory(unsigned int _size);
  unsigned int history() const;

public slots:
  void insertEvent(const QString& _stamp,
		   const QString& _domain, const QString& _type, const QString& _event);

protected slots:
  void selectionChanged(QListViewItem * _item);
  void excludeEvent(const QString& _domainName, const QString& _typeName);
  void includeEvent(const QString& _domainName, const QString& _typeName);

protected:
  virtual void hideEvent(QHideEvent * _event);

  void pruneHistory();
  static ACE_Time_Value eventTime(QListViewItem * _item);

  FileSet * fileSet_;
  QListView * list_;
  QListViewItem * last_;

  bool internalSetSelection_;

private:
  EventView(EventView const&);

  unsigned int history_;
};

inline
unsigned int
EventView::history() const {
  return history_;
}

#endif // EventView_h
