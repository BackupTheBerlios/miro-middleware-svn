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
#include <iostream>

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

void MainForm::mousePressEvent(QMouseEvent * /*mouse*/)
{
  mouseMoving=true;
//  mouse=mouse; // remove warning ;-)
}

void MainForm::mouseReleaseEvent(QMouseEvent * /*mouse*/)
{
  mouseMoving=false;

  Miro::VelocityIDL speed;

  speed.translation=0;
  speed.rotation=0;

  try {
    motion->setVelocity(speed);
  } catch (const Miro::EDevIO& e) {
    std::cout << e << e.what << std::endl;
  }

  //motion->limp();
//  mouse=mouse; // remove warning ;-)
}

void MainForm::mouseMoveEvent(QMouseEvent * mouse)
{
  if (!mouseMoving) return;

  int x=mouse->x();
  int y=mouse->y();

  x=x-125; //center
  y=y-125;
  std::cout << "(" << x << ", " << y << ")" << "   "; //std::endl;

  if (abs(x)<10) x=0;
  if (x>100) x=100;
  if (x<-100) x=-100;
  if (abs(y)<10) y=0;
  if (y>100) y=100;
  if (y<-100) y=-100;

  Miro::VelocityIDL speed;

  int mint,maxt;
  double minr,maxr;

  motion->getMinMaxVelocity(mint,maxt,minr,maxr);

//  speed.translation=-(y*maxt)/125;
  speed.translation=-(y*maxt)/100;
  
  //  speed.translation=-(y*20); //950)/200;
//  speed.rotation=-double(x)*3.1415927/200;
  speed.rotation=-double(x)*maxr/100;

  std::cout << "Transvel: " << speed.translation << " Rotvel: " << speed.rotation << std::endl;
  try {
    motion->setVelocity(speed);
  } catch (const Miro::EDevIO& e) {
    std::cout << e << e.what << std::endl;
  } catch (const Miro::EOutOfBounds& e) {
    std::cout << e << e.what << std::endl;
  }
}

void MainForm::paintEvent(QPaintEvent * /*pe*/)
{
  QPainter p(this);

  p.setPen(Qt::black);
  p.drawEllipse(115,115,20,20);
  p.drawRect(25,25,200,200);

//  pe=pe; //warning off ;-)
}
