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

