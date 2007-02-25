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
#include "miro/TimeHelper.h"
#include "miro/Log.h"
#include "idl/MotionC.h"
#ifdef MIRO_HAS_SPARROW99
#include "idl/KickerC.h"
#endif

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

#ifdef MIRO_HAS_SPARROW99
  inline void kick();
#endif
  
  Miro::Client client;
  Miro::Motion_var motion;
  bool mouseMoving;

};

#endif
