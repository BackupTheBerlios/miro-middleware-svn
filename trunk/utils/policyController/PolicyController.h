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
#ifndef PolicyController_hh
#define PolicyController_hh

#include "miro/Client.h"
#include "idl/BehaviourEngineC.h"

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
  void suspendPolicy();
  void resumePolicy();

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
  QPushButton * suspendButton;
  QPushButton * resumeButton;

  int iLoad_;
  int iSend_;
  int iTransition_;
  int iGlobalTransition_;
};

#endif
