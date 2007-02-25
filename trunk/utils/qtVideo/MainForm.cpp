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
#include "MainForm.h"

#include "miro/VideoHelper.h"
#include "miro/TimeHelper.h"
#include "miro/Log.h"

#include <qpainter.h>
#include <qimage.h>

#include <sys/shm.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;



std::string path()
{
  char * miro_log = ACE_OS::getenv("MIRO_LOG");
  std::string p;
  
  if (miro_log)
    p = miro_log;

  if (p.size() > 0 && p[p.size() - 1] != '/')
    p += "/";
  return p;
}



bool MainForm::remote = false;
bool MainForm::verbose = false;
std::string MainForm::interfaceName = "Video";




MainForm::MainForm(Miro::Client& _client, 
		   QWidget *parent, char const * _name) : 
  QDialog(parent, _name, false, WDestructiveClose),
  qImage_(new QImage()),
  client_(_client),
  imageBuffer_(NULL),
  ppmBuffer_(NULL),
  ppmOffset_(0),
  imageSize_(0),
  bpp_(32),
  fps(0),
  fpsTmp(0),
  bgr_(false),
  checkJpeg_(false),
  isJpeg_(false),
  colorTable_(NULL)
{

  timer = new QTimer;
  try {
    video_ = client_.resolveName<Miro::Video>(interfaceName.c_str());
    imageIDL_ = video_->connect(id_);

    initDialog();
    imageSize_ = Miro::getImageSize(imageIDL_->format);
    bpp_ = Miro::getPixelSize(imageIDL_->format.palette) * 8;

    if (bpp_ != 8 && bpp_ != 24 && bpp_ != 32)
      throw Miro::Exception("Unsupported image format.");

    lastTime_ = ACE_OS::gettimeofday();

    if (!remote) {
      imageBuffer_ = (unsigned char*)::shmat(imageIDL_->key, NULL, 0);
      if ((long)imageBuffer_ == -1)
	throw Miro::EDevIO();
    }

    std::ostringstream ostr;
    ostr << "P" << ((bpp_ == 8)? 5 : 6) << "\n" << imageIDL_->format.width << " " << imageIDL_->format.height << " 255\n";
    ppmOffset_ = strlen(ostr.str().c_str());
    ppmBuffer_ = new unsigned char[imageSize_ + ppmOffset_];

    memset(ppmBuffer_, 0, imageSize_ + ppmOffset_);
    memcpy(ppmBuffer_, ostr.str().c_str(), strlen(ostr.str().c_str()));

    if (bpp_ == 8) {
      colorTable_ = new QRgb[256];
      for (int i = 0; i < 256; ++i) {
	colorTable_[i] = qRgb(i, i, i);
      }
    }
  }
  catch(CORBA::Exception &e) {
    cerr << "CORBA Exception: " << e <<endl;;
    exit(-1);
  }
}



MainForm::~MainForm()
{
  if (imageBuffer_ != NULL && (long)imageBuffer_ != -1)
    shmdt((void*)imageBuffer_);

  if (video_.in() != NULL)
    video_->disconnect(id_);

  delete ppmBuffer_;
  delete colorTable_;
  delete qImage_;
}



void MainForm::initDialog()
{
  this->setGeometry(0, 0,
		    imageIDL_->format.width, imageIDL_->format.height + 50);

  buttonGrab = new QPushButton("Grab!",this,"button");
  buttonGrab->move((imageIDL_->format.width - buttonGrab->width()) / 2,
		   imageIDL_->format.height + 25 - buttonGrab->height() / 2);

  connect(buttonGrab, SIGNAL(clicked()), SLOT(slotGrabClicked()));
  connect(timer,SIGNAL(timeout()),SLOT(actualizeImage()));
  timer->start(0);
}



void MainForm::paintEvent(QPaintEvent * )
{
  QPainter p(this);

  if (!qImage_->isNull()) {
    p.drawImage(QPoint(0,0), *qImage_); 

    // draw frames per second status information
    char strFps[10];
    sprintf(strFps,"fps: %2i",fps);
    p.setBrush(p.backgroundColor());
    p.setPen(p.backgroundColor());
    p.drawRect(0, qImage_->height(), 100, qImage_->height() + 20);
    p.setPen(black);
    p.drawText(5,qImage_->height() + 15, QString(strFps));
  }
  else {
    cout << "no image to paint" << endl;
  }
}



void MainForm::actualizeImage()
{
  Miro::SubImageDataIDL_var remoteData;

  if (remote) {
    remoteData = 
      video_->exportWaitSubImage(imageIDL_->format.width, imageIDL_->format.height);
  }
  else {
    try {
      Miro::TimeIDL tC =
	video_->acquireNextImage(id_, bufferIndex_);

      ACE_Time_Value tA;
      Miro::timeC2A(tC, tA);
      MIRO_LOG_OSTR(LL_NOTICE, "image time stamp: " << tA);
    }
    catch (Miro::EOutOfBounds const& e) {
      cout << "Out of bounds exception:" << e.what << endl;
      abort();
    }
    imageOffset_ = imageIDL_->offset[bufferIndex_];
  }

  // calculate statistics
  ++fpsTmp;
  if (lastTime_ + ACE_Time_Value(1) < ACE_OS::gettimeofday()) { //1 second passed
    fps = fpsTmp;
    fpsTmp = 0;
    lastTime_ = ACE_OS::gettimeofday();
  }
  
  if (remote)
    memcpy(ppmBuffer_ + ppmOffset_, remoteData->get_buffer(), imageSize_);
  else
    memcpy(ppmBuffer_ + ppmOffset_, imageBuffer_ + imageOffset_, imageSize_);

  if (bpp_ == 32) {
    delete qImage_;
    qImage_ = new QImage(ppmBuffer_ + ppmOffset_, 
			 imageIDL_->format.width, imageIDL_->format.height, bpp_,
			 colorTable_, 256, QImage::IgnoreEndian);
  }
  else {
    // okay, here's a hack to recognize jpeg images. if we found one, we just ignore the
    // ppm header and use only the image data itself (which contains the jpeg header
    // already)
    if (!checkJpeg_) {
      checkJpeg_ = true;
      if ((ppmBuffer_[ppmOffset_] == 0xff) && (ppmBuffer_[ppmOffset_+1] == 0xd8) &&
	  qImage_->loadFromData(ppmBuffer_ + ppmOffset_, imageSize_, "JPEG")) {
	// if mime-magic applies and image loading works, we also have to set the window size and button pos
	this->setGeometry(0, 0, qImage_->width(), qImage_->height() + 50);
	buttonGrab->move((qImage_->width() - buttonGrab->width()) / 2,
			 qImage_->height() + 25 - buttonGrab->height() / 2);
	cout << "jpeg-format detected" << endl;
	isJpeg_ = true;
      }
    }

    if  (isJpeg_)
      qImage_->loadFromData(ppmBuffer_ + ppmOffset_, imageSize_, "JPEG");
    else
      qImage_->loadFromData(ppmBuffer_, imageSize_ + ppmOffset_, "PPM");
  }

  if (!remote)
    video_->releaseImage(id_, bufferIndex_);

  repaint(FALSE);
}



void MainForm::slotGrabClicked()
{
  if (qImage_) {
    std::string fileName = path() + "video_" + Miro::timeString() + ".ppm";
    if (qImage_->save(fileName.c_str(), "PPM") == false)
      std::cout << "image save failed." << endl;
  }
  else {
    std::cout << "no image to save." << endl;
  }
}
