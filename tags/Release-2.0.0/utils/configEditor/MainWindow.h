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
#ifndef MainWindow_h
#define MainWindow_h

#include "ConfigDocumentXML.h"

#include <qmainwindow.h>

// forward declarattions
class DocumentView;
class FileListDialog;
class ConfigFile;
namespace Miro {
  class Client;
}

//! Main class of the MainWindow application 
class MainWindow : public QMainWindow
{
  Q_OBJECT

  //----------------------------------------------------------------------------
  // private types
  //----------------------------------------------------------------------------

  //! The super class.
  typedef QMainWindow Super;

public:
  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------

  //! Initializing constructor.
  MainWindow(Miro::Client& _client);
  virtual ~MainWindow();

  //! Accessor method for the document view.
  DocumentView * view();

protected slots:
  //----------------------------------------------------------------------------
  // protected slots
  //----------------------------------------------------------------------------

  // file menu
  void slotGetFrom();
  void slotSendTo();

  // options menu
  void paramsDescriptions();

  // help menu
  void slotAbout();
  void slotAboutQt();

protected:
  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------
  void closeEvent(QCloseEvent *e);

  //----------------------------------------------------------------------------
  // protected data
  //----------------------------------------------------------------------------

  //! Editor configuration.
  ConfigFile * const config_;
  //! Configuration widget.
  DocumentView * view_;
  //! Configuration file.
  ConfigDocumentXML document_;
  //! Parameter file dialog.
  FileListDialog * configDialog_;

  //! CORBA client reference.
  Miro::Client& client_;
  //! CORBA interface name.
  QString interfaceName_;

private:
  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  MainWindow(MainWindow const&);
};

inline
DocumentView *
MainWindow::view() {
  return view_;
}

#endif

