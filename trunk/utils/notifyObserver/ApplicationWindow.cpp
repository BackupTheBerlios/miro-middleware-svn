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
#include "ApplicationWindow.h"
#include "EventChannelFactory.h"

#include "miro/Server.h"

#include <orbsvcs/CosNotifyChannelAdminC.h>

#include <qmenubar.h> 
#include <qpopupmenu.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qtimer.h>

#include <sstream>

ApplicationWindow::ApplicationWindow(Miro::Server& _server) :
  Super(0, "MainWindow"),
  server_(_server),
  listView_(new QListView(this, "list view")),
  timer_(new QTimer(this, "timer"))
{
  //-----------//
  // init menu //
  //-----------//

  // file menu
  QPopupMenu* menuFile = new QPopupMenu();
  menuBar()->insertItem("&File", menuFile);

  menuFile->insertItem("Resolve Channel Factory", this, SLOT(slotResolveFactory()));
  menuFile->insertSeparator();
  menuFile->insertItem("Quit", this, SLOT(close()));

  // help menu
  QPopupMenu* menuHelp = new QPopupMenu();
  menuBar()->insertSeparator();
  menuBar()->insertItem("&Help", menuHelp);
  
  menuHelp->insertItem("About PolicyEditor", this, SLOT(slotAbout()));
  menuHelp->insertItem("About Qt", this, SLOT(slotAboutQt()));
  
  // init view //
  listView_->setRootIsDecorated(true);
  listView_->addColumn("Object ID");
  listView_->addColumn("Type");

  connect(listView_, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint&, int)),
	  this, SLOT(contextMenu(QListViewItem *, const QPoint&, int)));

  setCentralWidget(listView_);

  timer_->start(1000);
  resolveFactory("NotifyEventChannelFactory");
}

void
ApplicationWindow::slotResolveFactory()
{
  bool ok = false;
  QString defaultName = "NotifyEventChannelFactory";
  QString text = QInputDialog::getText(
                    tr( "Resolve EventChannelFactory:" ),
                    tr( "Factory name:" ),
                    QLineEdit::Normal, defaultName, &ok, this );
  if ( ok && !text.isEmpty() ) {
    resolveFactory(text);
  }
}

void
ApplicationWindow::contextMenu(QListViewItem * _item, QPoint const& _pos, int _column)
{
  if (_item != NULL && _column == 0) {
    Item::ItemMap::const_iterator item = Item::itemMap().find(_item);
    if (item != Item::itemMap().end()) {
      QPopupMenu  menu(NULL, "ContextMenu");
      item->second->contextMenu(menu);

      menu.exec(_pos);
    }
  }
}


void 
ApplicationWindow::slotAbout()
{
  QMessageBox::about(this, 
		     "About NotifyObserver", 

		     "Notify Event Channel Observer Ver. 1.0\n Copyright 2004 Hans Utz");
}

void 
ApplicationWindow::slotAboutQt()
{
  QMessageBox::aboutQt(this, "About Qt");
}

void
ApplicationWindow::resolveFactory(QString const& _name)
{
  try {
    CosNaming::Name name;
    
    name.length(1);
    name[0].id = CORBA::string_dup(_name.latin1());
    CosNotifyChannelAdmin::EventChannelFactory_ptr factory =
      server_.resolveName<CosNotifyChannelAdmin::EventChannelFactory>(name);
    
    EventChannelFactory * f = 
      new EventChannelFactory(factory, listView_, this, _name);

    connect(timer_, SIGNAL(timeout()), f, SLOT(updateFactory()));
  }
  catch(CORBA::Exception& e) {
    std::ostringstream sstr;
    sstr << "CORBA exception: " << e << std::flush;

    QString error = sstr.str().c_str();
    QMessageBox::warning(this, 
			 "Resolve EventChannelFactory", 
			 "Error resolving factory " + _name + ".\n" + error);

  }
}
