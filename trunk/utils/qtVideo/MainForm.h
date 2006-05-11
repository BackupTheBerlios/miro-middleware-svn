// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MainForm_h
#define MainForm_h

#include "miro/Client.h"
#include "idl/VideoC.h"

#include <qwidget.h>
#include <qdialog.h>
#include <qtimer.h>
#include <qpushbutton.h>

#include <ace/Time_Value.h>

#include <string>

class MainForm : public QDialog  
{
  Q_OBJECT
 public: 
  MainForm(Miro::Client& _client,
	   QWidget *parent=0, const char *name = 0);
  ~MainForm();

  // static data
  static bool remote;
  static bool verbose;
  static std::string interfaceName;

	    public slots:
	    void actualizeImage();
  void slotGrabClicked();

 protected:
  void initDialog();
  void paintEvent(QPaintEvent * p);
  
  QTimer * timer;
  QPushButton * buttonGrab;
  QImage * qImage_;

  Miro::Client& client_;
  Miro::Video_var video_;
  Miro::ImageHandleIDL_var imageIDL_;

  CORBA::ULong id_;
  CORBA::ULong bufferIndex_;
  CORBA::ULong imageOffset_;

  unsigned char * imageBuffer_;
  unsigned char * ppmBuffer_;
  unsigned int ppmOffset_;

  int imageSize_;
  int bpp_;
  int fps;
  int fpsTmp;
  bool bgr_;
  bool checkJpeg_;
  bool isJpeg_;

  ACE_Time_Value lastTime_;

  QRgb * colorTable_; //used for gray images
};

#endif
