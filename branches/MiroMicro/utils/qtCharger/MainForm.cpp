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
// $Id: MainForm.cpp jaklitsch $
//
#include "MainForm.h"
#include <unistd.h>
#include <iostream>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;

MainForm::MainForm(int argc, char* argv[], QWidget *parent, const char *name) : 
  QDialog(parent,name,false,WDestructiveClose),
  client(argc,argv),
  charger(NULL),
  battery(NULL)
{

  initDialog();

  try {
    charger = Miro::resolve_name<Miro::Charger>(client, "Charger");
    battery = Miro::resolve_name<Miro::Battery>(client, "Battery");
  }
  catch (...)
  {
    exit(-1);
  }
}

MainForm::~MainForm()
{
}

void MainForm::initDialog()
{
  buttonDeploy=new QPushButton("Deploy",this,"button");
  buttonDeploy->move(150,75);
  buttonStatus=new QPushButton("Status",this,"button");
  buttonStatus->move(350,75);
//  buttonDisplay=new QButton(this,"nc");
//  buttonDisplay->move(350,75);
  buttonInhibit=new QPushButton("Inhibit",this,"button");
  buttonInhibit->move(150,125);


  connect(buttonDeploy,SIGNAL(pressed()),SLOT(slotDeployOn()));
  connect(buttonDeploy,SIGNAL(released()),SLOT(slotDeployOff()));
  connect(buttonStatus,SIGNAL(pressed()),SLOT(slotStatusOn()));
  connect(buttonInhibit,SIGNAL(pressed()),SLOT(slotInhibitOn()));
  connect(buttonInhibit,SIGNAL(released()),SLOT(slotInhibitOff()));
}

void MainForm::slotDeployOn()
{
    charger->deployCharger();
    cout << charger->getChargerStatus() << endl;
}
void MainForm::slotDeployOff()
{
}
void MainForm::slotStatusOn()
{
    cout << charger->getChargerStatus() << endl;
    cout << battery->getVoltage() << endl;
}
void MainForm::slotInhibitOn()
{
  charger->retractCharger();
  cout << charger->getChargerStatus() << endl;
}
void MainForm::slotInhibitOff()
{
}
