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
#include "idl/MotionC.h"
#include "idl/OdometryC.h"

class MainForm : public QDialog  
{
  Q_OBJECT
public: 
  MainForm(int argc, char* argv[], QWidget *parent=0, const char *name=0);
  ~MainForm();

public slots:
  void slotForwardOn();
  void slotForwardOff();
  void slotFastForwardOn();
  void slotFastForwardOff();
  void slotBackwardOn();
  void slotBackwardOff();
  void slotLeftOn();
  void slotLeftOff();
  void slotRightOn();
  void slotRightOff();
  void slotStopClicked();

protected:
  void initDialog();

  QPushButton *buttonFastForward;
  QPushButton *buttonForward;
  QPushButton *buttonBackward;
  QPushButton *buttonLeft;
  QPushButton *buttonRight;
  QPushButton *buttonStop;

  Miro::Client client;
  Miro::Motion_var motion;
  Miro::Odometry_var odometry;
};

#endif
