// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "MainForm.h"
#include <unistd.h>

MainForm::MainForm(int argc, char* argv[], QWidget *parent, const char *name) : 
  QDialog(parent,name,false,WDestructiveClose),
  client(argc,argv),
  motion(NULL),
  odometry(NULL)
{

  initDialog();

  try {
    motion = Miro::resolve_name<Miro::Motion>(client,"Motion");
    odometry = Miro::resolve_name<Miro::Odometry>(client,"Odometry");
  }
  catch (...)
  {
    motion=NULL;
    exit(-1);
  }
}

MainForm::~MainForm()
{
}

void MainForm::initDialog()
{
  buttonFastForward=new QPushButton("Fast Forward",this,"button");
  buttonFastForward->move(150,25);
  buttonForward=new QPushButton("Forward",this,"button");
  buttonForward->move(150,75);
  buttonBackward=new QPushButton("Backward",this,"button");
  buttonBackward->move(150,125);
  buttonLeft=new QPushButton("Left",this,"button");
  buttonLeft->move(50,100);
  buttonRight=new QPushButton("Right",this,"button");
  buttonRight->move(250,100);
  buttonStop=new QPushButton("STOP!",this,"button");
  buttonStop->move(150,100);


  connect(buttonForward,SIGNAL(pressed()),SLOT(slotForwardOn()));
  connect(buttonForward,SIGNAL(released()),SLOT(slotForwardOff()));
  connect(buttonFastForward,SIGNAL(pressed()),SLOT(slotFastForwardOn()));
  connect(buttonFastForward,SIGNAL(released()),SLOT(slotFastForwardOff()));
  connect(buttonBackward,SIGNAL(pressed()),SLOT(slotBackwardOn()));
  connect(buttonBackward,SIGNAL(released()),SLOT(slotBackwardOff()));
  connect(buttonLeft,SIGNAL(pressed()),SLOT(slotLeftOn()));
  connect(buttonLeft,SIGNAL(released()),SLOT(slotLeftOff()));
  connect(buttonRight,SIGNAL(pressed()),SLOT(slotRightOn()));
  connect(buttonRight,SIGNAL(released()),SLOT(slotRightOff()));
  connect(buttonStop,SIGNAL(pressed()),SLOT(slotStopClicked()));
}

void MainForm::slotFastForwardOn()
{
  Miro::VelocityIDL speed=odometry->getVelocity();
  speed.translation=400;
  motion->setVelocity(speed);
}
void MainForm::slotFastForwardOff()
{
  motion->limp();
}

void MainForm::slotForwardOn()
{
  Miro::VelocityIDL speed=odometry->getVelocity();
  speed.translation=200;
  motion->setVelocity(speed);
}
void MainForm::slotForwardOff()
{
  motion->limp();
}

void MainForm::slotBackwardOn()
{
  Miro::VelocityIDL speed=odometry->getVelocity();
  speed.translation=-200;
  motion->setVelocity(speed);
}
void MainForm::slotBackwardOff()
{
  motion->limp();
}

void MainForm::slotLeftOn()
{
  Miro::VelocityIDL speed=odometry->getVelocity();
  speed.rotation=1;
  motion->setVelocity(speed);
}
void MainForm::slotLeftOff()
{
  motion->limp();
}

void MainForm::slotRightOn()
{
  Miro::VelocityIDL speed=odometry->getVelocity();
  speed.rotation=-1;
  motion->setVelocity(speed);
}
void MainForm::slotRightOff()
{
  motion->limp();
}


void MainForm::slotStopClicked()
{
  motion->limp();
  cout << "Emergency stop!" << endl;
}
