// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1998, 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PolicyEditor_h
#define PolicyEditor_h

#include "PolicyDocument.h"

#include "miro/Client.h"

#include <qmainwindow.h>

// forward declarattions
class FileListDialog;
class PolicyViewClass;

//! Main class of the PolicyEditor application 
class PolicyEditorClass : public QMainWindow
{
  Q_OBJECT
  
public:
  PolicyEditorClass(Miro::Client& _client);

  void newPolicy();
  void openPolicy(const QString& _name);
  bool savePolicy();
  bool savePolicyAs();

protected slots:
  // file menu
  void slotNew();
  void slotLoad();
  void slotSave();
  void slotSaveAs();
  void slotSendTo();
  // options menu
  void behaviourDescriptions();
  //void slotConfiguration();
  // help menu
  void slotAbout();
  void slotAboutQt();

protected:
  void defaultView();
  bool saveIfModified();
  void closeEvent(QCloseEvent *e);

  Miro::Client&       client_;
  PolicyConfigClass   config_;
  PolicyDocumentClass document_;
  PolicyViewClass *   view_;
  FileListDialog *      configDialog_;

  QString             robot_;
};

#endif

