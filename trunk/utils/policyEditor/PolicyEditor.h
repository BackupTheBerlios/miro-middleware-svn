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

#include "miro/Client.h"

#include "PolicyDocument.h"
#include "PolicyView.h"

#include <qmainwindow.h>

/** main class of the PolicyEditor application */
class PolicyEditorClass : public QMainWindow
{
  Q_OBJECT
private:
  Miro::Client&       client_;
  PolicyDocumentClass Document;
  PolicyViewClass*    View;
  QString             policyFileName;
  QString robot_;

private slots:
  void slotNew();
  void slotLoad();
  void slotSave();
  void slotSaveAs();
  void slotSendTo();
  //void slotConfiguration();
  void slotAbout();
  void slotAboutQt();
  void getBehaviourDescriptionFileName();
  void setBehaviourDescriptionFileName();
  void quit();
  
protected:
  void closeEvent(QCloseEvent *e);

public:
  PolicyEditorClass(int argc, char** argv, Miro::Client& _client);
};

#endif

