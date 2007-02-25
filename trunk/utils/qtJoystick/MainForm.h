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
