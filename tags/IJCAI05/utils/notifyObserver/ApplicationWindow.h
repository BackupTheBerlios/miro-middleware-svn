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
#ifndef notifyObserver_ApplicationWindow_h
#define notifyObserver_ApplicationWindow_h

#include <qmainwindow.h>

// forward declarations
class QListView;
class QListViewItem;
class QTimer;

namespace Miro
{
  class Server;
}

class ApplicationWindow : public QMainWindow
{
  Q_OBJECT;

  typedef QMainWindow Super;

public:
  ApplicationWindow(Miro::Server& _server);

protected slots:
  void slotResolveFactory();
  void contextMenu(QListViewItem * _item, QPoint const&, int _column);
  void slotAbout();
  void slotAboutQt();

protected:
  void resolveFactory(QString const& _name);

  Miro::Server& server_;

  QListView * listView_;
  QTimer * timer_;
};

#endif // notifyObserver_MainWindow_h
