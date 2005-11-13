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
  speed.translation = 0;
  speed.rotation = 0;

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

void MainForm::keyPressEvent(QKeyEvent * ke)
{
  Miro::VelocityIDL newSpeed;
  newSpeed.rotation = this->speed.rotation;
  newSpeed.translation = this->speed.translation;
  int mint,maxt;
  double minr,maxr;

  const double keySpeed = 0.5;
  const double keyRotation = 0.3;

  motion->getMinMaxVelocity(mint,maxt,minr,maxr);

  int k = ke->key();
  if (k == Key_Up) {
    newSpeed.translation = maxt*keySpeed;
  } else if (k == Key_Down) {
    newSpeed.translation = mint*keySpeed;
  } else if (k == Key_Left) {
    newSpeed.rotation = maxr*keyRotation;
  } else if (k == Key_Right) {
    newSpeed.rotation = minr*keyRotation;
#ifdef MIRO_HAS_SPARROW99
  } else if (k == Key_Space) {
    kick();
#endif
  } else {
    ke->ignore();
  }
  
  if (newSpeed.rotation != this->speed.rotation ||
      newSpeed.translation != this->speed.translation) {
    this->speed.rotation = newSpeed.rotation;
    this->speed.translation = newSpeed.translation;
    motion->setVelocity(this->speed);
  }
	  
}

#ifdef MIRO_HAS_SPARROW99
inline void MainForm::kick()
{
  try {

    Miro::Kicker_var kicker = client.resolveName<Miro::Kicker>("Kicker");
    ACE_Time_Value aT;
    aT.msec(200);
    Miro::TimeIDL cT;
    Miro::timeA2C(aT, cT);
    kicker->kick(cT);
    
  } catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Uncaught CORBA exception:" << std::endl << e);
  }
}
#endif

void MainForm::keyReleaseEvent(QKeyEvent * ke)
{
  Miro::VelocityIDL newSpeed;
  newSpeed.rotation = this->speed.rotation;
  newSpeed.translation = this->speed.translation;
  int k = ke->key();
  if (k == Key_Up) {
    newSpeed.translation = 0;
  } else if (k == Key_Down) {
    newSpeed.translation = 0;
  } else if (k == Key_Left) {
    newSpeed.rotation = 0;
  } else if (k == Key_Right) {
    newSpeed.rotation = 0;
  } else {
    ke->ignore();
  }
  
  if (newSpeed.rotation != this->speed.rotation ||
      newSpeed.translation != this->speed.translation) {
    this->speed.rotation = newSpeed.rotation;
    this->speed.translation = newSpeed.translation;
    motion->setVelocity(this->speed);
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
