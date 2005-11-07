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

#ifndef MAINFORM_H
#define MAINFORM_H

#include <qwidget.h>
#include <qdialog.h>
#include <qpushbutton.h>

#include "miro/Client.h"
#include "miro/TimeHelper.h"
#include "miro/Log.h"
#include "idl/MotionC.h"
#include "idl/KickerC.h"

class QPad : public QWidget {
protected:
  /*  void paintEvent(QPaintEvent * p);
  virtual void mousePressEvent(QMouseEvent * mouse);
  virtual void mouseReleaseEvent(QMouseEvent * mouse);
  virtual void mouseMoveEvent(QMouseEvent * mouse); */
  bool mouseMoving;

};

class MainForm : public QDialog  {
  Q_OBJECT
public: 
  MainForm(int argc, char* argv[], QWidget *parent=0, const char *name=0);
  ~MainForm();

  /*public slots:
  void slotMouseOn(QMouseEvent * mouse);
  void slotMouseOff(QMouseEvent * mouse);
  void slotMouseMove(QMouseEvent * mouse);
  */
protected:
  QPad * pad;

  void initDialog();
  void paintEvent(QPaintEvent * p);
  virtual void mousePressEvent(QMouseEvent * mouse);
  virtual void mouseReleaseEvent(QMouseEvent * mouse);
  virtual void mouseMoveEvent(QMouseEvent * mouse); 
  virtual void keyPressEvent(QKeyEvent * ke); 
  virtual void keyReleaseEvent(QKeyEvent * ke); 
  Miro::VelocityIDL speed;

  void kick();
  
  Miro::Client client;
  Miro::Motion_var motion;
  bool mouseMoving;

};

#endif
