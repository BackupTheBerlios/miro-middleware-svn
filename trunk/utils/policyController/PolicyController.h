// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Nix (Nix Is eXtreme)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// $Date$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PolicyController_hh
#define PolicyController_hh

#include "miro/Client.h"
#include "miro/BehaviourEngineC.h"

#include <qwidget.h>

// forward declarations
class QString;
class QMenuBar;
class QPopupMenu;
class QPushButton;

class ControllerWidget : public QWidget
{
  Q_OBJECT
public:
  ControllerWidget(Miro::Client& _client, bool _start = false,
		   QWidget *parent = 0, const char *name = 0);

  void connectToRobot(const QString& _robot, bool _silent = false);
  void sendPolicy(const QString& _policy);
  void loadPolicy(const QString& _policy);
  void sendTransition(const QString& _transition);
  void sendGlobalTransition(const QString& _pattern);

protected slots:
  void robot();
  void load();
  void send();

  void openPolicy();
  void closePolicy();

  void sendTransition();
  void sendGlobalTransition();

protected:
  void setItemsEnable(bool toggle);

  Miro::Client& client_;
  Miro::BehaviourEngine_var engine_;

  QString robotName_;
  QString policyName_;
  QString policyPath_;
  QString transition_;
  QString pattern_;

  bool start_;

  QMenuBar *menuBar;
  QPopupMenu *fileMenu;
  QPopupMenu *editMenu;

  QPushButton *playButton;
  QPushButton *stopButton;

  int iLoad_;
  int iSend_;
  int iTransition_;
  int iGlobalTransition_;
};

#endif
