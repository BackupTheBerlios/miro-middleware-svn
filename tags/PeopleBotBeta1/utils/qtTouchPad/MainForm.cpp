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
#include <qpainter.h>

MainForm::MainForm(int argc, char* argv[], QWidget *parent, const char *name) : 
  QDialog(parent,name,false,WDestructiveClose),
  client(argc,argv),
  mouseMoving(false)
{

  initDialog();

  try {
    motion=Miro::resolve_name<Miro::Motion>(client,"Motion");
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
  this->setGeometry(0,0,250,250);

}

void MainForm::mousePressEvent(QMouseEvent * mouse)
{
  mouseMoving=true;
  mouse=mouse; // remove warning ;-)
}
void MainForm::mouseReleaseEvent(QMouseEvent * mouse)
{
  mouseMoving=false;
  motion->limp();
  mouse=mouse; // remove warning ;-)
}

void MainForm::mouseMoveEvent(QMouseEvent * mouse)
{
  if (!mouseMoving) return;

  int x=mouse->x();
  int y=mouse->y();

  x=x-125; //center
  y=y-125;

  if (abs(x)<10) x=0;
  if (x>100) x=100;
  if (x<-100) x=-100;
  if (abs(y)<10) y=0;
  if (y>100) y=100;
  if (y<-100) y=-100;

  Miro::VelocityIDL speed;

  speed.translation=-(y*950)/200;
  speed.rotation=-double(x)*3.1415927/200;

  motion->setVelocity(speed);
}

void MainForm::paintEvent(QPaintEvent * pe)
{
  QPainter p(this);

  p.setPen(Qt::black);
  p.drawEllipse(115,115,20,20);
  p.drawRect(25,25,200,200);

  pe=pe; //warning off ;-)
}
