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
