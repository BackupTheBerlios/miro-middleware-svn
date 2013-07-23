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
