// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This is a graphical interface for deploy and retract the buildin charger 
// in the p3dx and for the new developed charger for the p3at
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; 
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// $Id: MainForm.h jaklitsch $
//
//

#ifndef MAINFORM_H
#define MAINFORM_H

#include <qwidget.h>
#include <qdialog.h>
#include <qpushbutton.h>

#include "miro/Client.h"
#include "idl/BatteryC.h"
#include "idl/ChargerC.h"

class MainForm : public QDialog  
{
  Q_OBJECT
public: 
  MainForm(int argc, char* argv[], QWidget *parent=0, const char *name=0);
  ~MainForm();

public slots:
  void slotDeployOn();
  void slotDeployOff();
  void slotStatusOn();
  void slotInhibitOn();
  void slotInhibitOff();

protected:
  void initDialog();

  QPushButton *buttonDeploy;
  QPushButton *buttonInhibit;
  QPushButton *buttonStatus;
  QButton *buttonDisplay;

  Miro::Client client;
  Miro::Charger_var charger;
  Miro::Battery_var battery;
};

#endif
