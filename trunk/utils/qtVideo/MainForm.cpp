// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "MainForm.h"

#include "miro/VideoHelper.h"
#include "miro/TimeHelper.h"

#include <qpainter.h>
#include <qimage.h>

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
      if ((int)imageBuffer_ == -1)
	throw Miro::EDevIO();
    }

    char ppmHeader[32]; // should be 15+1, but just in case...
    sprintf(ppmHeader,"P%i\n%3i %3i %3i\n",
	    ((bpp_ == 8)? 5 : 6),
	    imageIDL_->format.width,
	    imageIDL_->format.height, 255);
    ppmOffset_ = strlen(ppmHeader);
    ppmBuffer_ = new unsigned char[imageSize_ + ppmOffset_];

    memset(ppmBuffer_, 0, imageSize_ + ppmOffset_);
    memcpy(ppmBuffer_, ppmHeader, strlen(ppmHeader));

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
  catch (...)
  {
    cerr << "Error Somewere" << endl;;
    exit(-1);
  }
}

MainForm::~MainForm()
{
  if (imageBuffer_ != NULL && (int)imageBuffer_ != -1)
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

  if (!qImage_->isNull()) 
    p.drawImage(QPoint(0,0), *qImage_); 
  else {
    cout << "no image to paint" << endl;
  }

  // draw frames per second status information

  char strFps[10];
  sprintf(strFps,"fps: %2i",fps);
  p.setBrush(p.backgroundColor());
  p.setPen(p.backgroundColor());
  p.drawRect(0, imageIDL_->format.height, 100, imageIDL_->format.height + 20);
  p.setPen(black);
  p.drawText(5,imageIDL_->format.height + 15, QString(strFps));
}

void MainForm::actualizeImage()
{
  Miro::SubImageDataIDL_var remoteData;

  if (remote) {
    remoteData = 
      video_->exportWaitSubImage(imageIDL_->format.width, imageIDL_->format.height);
  }
  else {
    video_->acquireNextImage(id_, bufferIndex_);
    imageOffset_ = imageIDL_->offset[bufferIndex_];
  }


  // calculate statistics
  // TODO: should be improved
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
