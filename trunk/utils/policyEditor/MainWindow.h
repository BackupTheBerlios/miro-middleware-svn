// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1998, 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MainWindow_h
#define MainWindow_h

#include "PolicyDocumentXML.h"

#include <qmainwindow.h>

// forward declarattions
class DocumentView;
class FileListDialog;
class ConfigFile;
class PolicyView;
namespace Miro {
  class Client;
}
class QListViewItem;

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
  void slotSendTo();

  // options menu
  void behaviourDescriptions();
  //void slotConfiguration();

  // help menu
  void slotAbout();
  void slotAboutQt();

  // document list view connection to policy view
  void slotNewSelection(QListViewItem * _item);

protected:
  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------
  void closeEvent(QCloseEvent *e);

  //----------------------------------------------------------------------------
  // protected data
  //----------------------------------------------------------------------------

  //! CORBA client reference.
  Miro::Client& client_;
  //! Editor configuration.
  ConfigFile * const config_;
  //! Splitter widget.
  QWidget * splitter_;
  //! Document widget.
  DocumentView * documentView_;
  //! Policy file widget.
  PolicyDocumentXML document_;
  //! Policy view widget.
  PolicyView * policyView_;

  //! Parameter file dialog.
  FileListDialog * configDialog_;

  QString robot_;

private:
  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  MainWindow(MainWindow const&);
};

inline
DocumentView *
MainWindow::view() {
  return documentView_;
}

#endif

