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
#ifndef PolicyEditor_h
#define PolicyEditor_h

#include "DocumentXML.h"

#include <qmainwindow.h>

// forward declarattions
class FileListDialog;
class ConfigFile;
class PolicyViewClass;
class QListView;
namespace Miro {
  class Client;
}

//! Main class of the PolicyEditor application 
class PolicyEditor : public QMainWindow
{
  Q_OBJECT
  
public:
  PolicyEditor(Miro::Client& _client);

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

  Miro::Client& client_;
  ConfigFile * const config_;
  DocumentXML document_;

  QListView * list_;
  PolicyViewClass * view_;
  FileListDialog * configDialog_;

  QString robot_;

private:
  //----------------------------------------------------------------------------
  // hidden methods
  //----------------------------------------------------------------------------
  PolicyEditor(PolicyEditor const&);
};

#endif

