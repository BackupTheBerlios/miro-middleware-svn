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
#include <qimage.h>
#include "video/VideoDevice.h"

using namespace Video;


  string path()
  {
    char * miro_log = ACE_OS::getenv("MIRO_LOG");
    string p;

    if (miro_log)
	p = miro_log;

    if (p.size() > 0 && p[p.size() - 1] != '/')
      p += "/";
    return p;
  }


  string createFileName()
  {
    ACE_Date_Time dt;
    dt.microsec(ACE_OS::gettimeofday().usec());

    char buffer[256];
    memset(buffer, 0, 256);
    ostrstream ostr(buffer, 255);

    ostr << dt.year() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.month() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.day() << "-";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.hour() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.minute() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << dt.second() << ".";
    ostr.width(2);
    ostr.fill('0');
    ostr << (dt.microsec() / 10000)
	 << ".ppm";

    return string(buffer);
  }


bool operator>(const ACE_Date_Time& a, const ACE_Date_Time& b) {
  //  cout << a.hour() << ":" << a.minute() << ":" << a.second() << "." << a.microsec();
  //  cout << "  ?>  ";
  //  cout << b.hour() << ":" << b.minute() << ":" << b.second() << "." << b.microsec();
  //  cout << endl;

  if (a.year()>b.year()) { return true; }
  if (a.year()<b.year()) { return false; }
  if (a.month()>b.month()) { return true; }
  if (a.month()<b.month()) { return false; }
  if (a.day()>b.day()) { return true; }
  if (a.day()<b.day()) { return false; }
  if (a.hour()>b.hour()) { return true; }
  if (a.hour()<b.hour()) { return false; }
  if (a.minute()>b.minute()) { return true; }
  if (a.minute()<b.minute()) { return false; }
  if (a.second()>b.second()) { return true; }
  if (a.second()<b.second()) { return false; }
  if (a.microsec()>b.microsec()) { return true; }
  return false;

}


MainForm::MainForm(int argc, char* argv[], QWidget *parent, const char *name) : 
  QDialog(parent,name,false,WDestructiveClose),
  client(argc,argv),
  image(NULL),
  ppm(NULL),
  imageLength(0),
  direction(1),
  bpp(32),
  fps(0),
  fpsTmp(0),
  bgr(false),
  colorTable(NULL)
{
  
  timer=new QTimer;
  try {
    video=Miro::resolve_name<Miro::Video>(client,"Video");
    imageIDL = video->connect();

    initDialog();
    imageLength=imageIDL.width*imageIDL.height;


    lastTime.microsec(ACE_OS::gettimeofday().usec());
    lastTime.second(lastTime.second()+1);

    if      (imageIDL.palette==paletteRGB)  {bpp=24; bgr=false;}
    else if (imageIDL.palette==paletteBGR)  {bpp=24; bgr=true;}
    else if (imageIDL.palette==paletteRGBA) {bpp=32; bgr=false;}
    else if (imageIDL.palette==paletteABGR) {bpp=32; bgr=true;}
    else if (imageIDL.palette==paletteGrey) {bpp=8;}

    image = (unsigned char*)::shmat(imageIDL.key, NULL, 0);
    if ((int)image == -1)
      throw Miro::EDevIO();
    ppm=(unsigned char*)malloc((imageLength*4+15)*sizeof(char));
    //must be viewed in rgba format, so... get enough memory

    char tmp[32]; //should be 15+1, but just in case...
    sprintf(tmp,"P6\n%3i %3i %3i\n",imageIDL.width,imageIDL.height,255);
    memcpy(ppm,tmp,strlen(tmp)+1);

    if (bpp==8) {
      colorTable=(QRgb*)malloc(256*sizeof(QRgb));
      for(int i=0; i<256; i++) {
	colorTable[i]=qRgb(i,i,i);
      }
    }

  }
  catch(CORBA::Exception &e) {
    video=NULL;
    cerr << "CORBA Exception: " << e <<endl;;
    exit(-1);
  }
  catch (...)
  {
    video=NULL;
    cerr << "Error Somewere" << endl;;
    exit(-1);
  }
}

MainForm::~MainForm()
{
  if (ppm!=NULL) free(ppm);
  if (colorTable!=NULL) free(colorTable);
}

void MainForm::initDialog()
{
  this->setGeometry(0,0,imageIDL.width,imageIDL.height+50);
  buttonGrab=new QPushButton("Grab!",this,"button");
  buttonGrab->move((imageIDL.width-buttonGrab->width())/2,imageIDL.height+25-buttonGrab->height()/2);

  connect(buttonGrab, SIGNAL(clicked()), SLOT(slotGrabClicked()));
  connect(timer,SIGNAL(timeout()),SLOT(actualizeImage()));
  timer->start(0);

}

void MainForm::paintEvent(QPaintEvent * pe)
{
  

  QPainter p(this);

  //  if ((bpp==32)||(bpp==8)) {
  int bpp2=(bpp==24)?32:bpp;

  QImage imIn((bpp==8)?image:ppm,imageIDL.width,imageIDL.height,bpp2,colorTable,256,QImage::IgnoreEndian);
  QImage im((!bgr)?imIn.swapRGB():imIn); //if original image is bgr, don't swap

  if (!im.isNull()) p.drawImage(QPoint(0,0),im); 
    //  }
  else {
    //slow but safe...

    int x,y;

    if (direction<=0) direction=-1;
    else direction=1;
    
    int i=(direction<0)?imageLength*bpp/8-bpp/8+1:1;
    int j;
    
    for (x=0,y=0,j=0;j<imageLength*bpp/8;j+=bpp/8,i+=direction*bpp/8) {
      p.setPen(QColor(image[i],image[i+1],image[i+2]));
      p.drawLine(x,y,x,y); //I cannot draw single pixels, so...
      x++;
      if (x>=imageIDL.width){
	x=0; y++;
      }
    }
  }

  char strFps[10];
  sprintf(strFps,"fps: %2i",fps);
  p.setBrush(p.backgroundColor());
  p.setPen(p.backgroundColor());
  p.drawRect(0,imageIDL.height,100,imageIDL.height+20);
  p.setPen(black);
  p.drawText(5,imageIDL.height+15,QString(strFps));

  pe=pe; //warning off ;-)
}

void MainForm::actualizeImage()
{
  ACE_Date_Time dt;

  video->getWaitImage(imageIDL);
  fpsTmp++;

  if (dt>lastTime) { //1 second passed
    fps=fpsTmp;
    fpsTmp=0;
    lastTime=dt;
    lastTime.second(lastTime.second()+1);
  }

  if (bpp==32) memcpy(ppm+((bgr)?16:15),image,imageLength*(bpp/8)*sizeof(char));
  if (bpp==24) {
    int i=0;
    unsigned char* imIn=image;
    unsigned char* imOut=ppm+15;
    for(i=0; i<imageLength; i++) {
      imOut++;          // Alpha
      *imOut++=*imIn++; // R
      *imOut++=*imIn++; // G
      *imOut++=*imIn++; // B
    }
  }
  repaint(FALSE);
  
}

void MainForm::slotGrabClicked()
{
  string fileName=path()+string("video_")+createFileName();

  cout << "Grabbing to " << fileName << endl;

  FILE* imgFile = fopen(fileName.c_str(),"wb");
  unsigned char* buffer=image;
  
  if (bpp==8) fprintf(imgFile,"P5\n%d %d 255\n",imageIDL.width,imageIDL.height);
  else fprintf(imgFile,"P6\n%d %d 255\n",imageIDL.width,imageIDL.height);

  for (int i=0; i<imageLength; i++) {
    if (bgr) { //if original is bgr, save as rgb
      if (bpp!=8) fwrite(buffer+2, sizeof(char), 1, imgFile); //R
      if (bpp!=8) fwrite(buffer+1, sizeof(char), 1, imgFile); //G
      fwrite(buffer, sizeof(char), 1, imgFile);   //B or grey
      if (bpp==32) buffer++; //Alpha
      buffer+=(bpp==8)?1:3; //increase 1 or 3 bytes for 8 or 24/32 bpp images
    } else {
      if (bpp==32) buffer++; //Alpha
      if (bpp!=8) fwrite(buffer++, sizeof(char), 1, imgFile); //R or grey
      if (bpp!=8) fwrite(buffer++, sizeof(char), 1, imgFile); //G
      fwrite(buffer++, sizeof(char), 1, imgFile); //B
    }
  }
  fclose(imgFile);

}
