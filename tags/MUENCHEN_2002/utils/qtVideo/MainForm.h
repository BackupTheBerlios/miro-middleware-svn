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
#include <qtimer.h>
#include <qpushbutton.h>

#include <ace/Date_Time.h>

#include "miro/Client.h"
#include "miro/VideoC.h"

class MainForm : public QDialog  {
  Q_OBJECT
public: 
  MainForm(int argc, char* argv[], QWidget *parent=0, const char *name=0);
  ~MainForm();

public slots:
  void actualizeImage();
  void slotGrabClicked();

protected:

  QTimer *timer;
  QPushButton *buttonGrab;

  void initDialog();
  void paintEvent(QPaintEvent * p);
  
  Miro::Client client;
  Miro::Video_var video;

  unsigned char* image;
  unsigned char* ppm;
  int imageLength;
  int direction; //indicates if the image should be flipped (rotated camera)
  int bpp;
  int fps;
  int fpsTmp;
  bool bgr;
  Miro::ImageHandleIDL imageIDL;
  ACE_Date_Time lastTime;
  QRgb *colorTable; //used for gray images

};

#endif
