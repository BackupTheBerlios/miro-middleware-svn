// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "VideoConsumer.h"
#include "VideoConnection.h"
#include "VideoDevice.h"
#include "VideoImpl.h"
#include "VideoConfig.h"

#include "miro/Exception.h"
#include "miro/TimeHelper.h"

#include <algorithm>

#undef DEBUG
#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
  ACE_Time_Value Consumer::maxWait(0, 500000);

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Connection& _connection,
		     VideoImpl * _pGrabber, 
		     ACE_Sched_Params * pschedp) :
    connection(_connection),
    pGrabber(_pGrabber),
    mutex(),
    cond(mutex),
    pCurrentImageData(NULL),
    schedp_(ACE_SCHED_OTHER, 0)

  {
    DBG(cout << "Constructing VideoConsumer." << endl);
    if (pschedp) 
      schedp_ = (*pschedp);
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//
  Consumer::~Consumer()
  { 
    DBG(cout << "Destructing VideoConsumer." << endl);

  }

  int 
  Consumer::svc()
  {
    cout << "Image size:" << getImageSize() << endl;
    pCurrentImageData = NULL;

    int pixelSize=connection.parameters.pixelSize;

    if (ACE_OS::sched_params(schedp_) == -1) {
      std::cerr << "[Video::Consumer] Could not set sched parameters." << endl 
		<< "[Video::Consumer] Maybe suid root is missing." << endl
		<< "[Video::Consumer] Will work on default sched policy" << endl;
    }

    ACE_Time_Value prevTimeStamp = ACE_OS::gettimeofday();
    int counter = 49;
    double deltaT[50];
    while (!canceled())
    {
      ACE_Time_Value timeStamp;
      try {
	void * pNextImageData = connection.videoDevice.grabImage(timeStamp);
	Miro::timeA2C(timeStamp, timeStamp_);

	/**************
	 *
	 * Flip image if camera is upside-down
	 * 
	 */
	
	unsigned char* image=(unsigned char *)pNextImageData;

	if (connection.parameters.upsideDown!=0) {
	  int i=0,j=0;
	  int imgSize=connection.parameters.width * connection.parameters.height;
	  for (i=0; i<imgSize/2; i++) {
	    //	    cout << i << " " << std::flush;
	    for (j=0; j<pixelSize; j++) {
	      int index=pixelSize*imgSize-pixelSize-(i*pixelSize)+j;
	      //save value from end
	      unsigned char tmp=image[index];
	      //copy value beginning -> end
	      image[index]=image[(i*pixelSize)+j];
	      //copy saved value(end)->beginning
	      image[(i*pixelSize)+j]=tmp;
	    }
	  }
	}


	/****
	 * 
	 * End of flipping
	 * 
	 */

	// lock();
	pCurrentImageData = pNextImageData;
	// release();
      }
      catch(Miro::CException& e) {
	cout << "VideoConsumer::svc() caught Miro::CException: " << endl
	     << e << endl;
      }
      catch(Miro::Exception& e) {
	cout << "VideoConsumer::svc() caught Miro::Exception: " << e << endl;
      }
      cond.broadcast();

      // jitter statistics
      ACE_Time_Value dT = timeStamp - prevTimeStamp;
      deltaT[counter] = (double)dT.sec() + (double)dT.usec() / 1000000.;
      prevTimeStamp = timeStamp;
      --counter;

      if (counter < 0) {
	double mean = deltaT[0];
	for (int i = 49; i > 0; --i)
	  mean += deltaT[i];
	mean /= 50.;

	double var = 0.;
	for (int i = 49; i >= 0; --i)
	  var += (deltaT[i] - mean) * (deltaT[i] - mean);
	var /= 49.;
	cout << "VideoConsumer: mean=" << mean << "sec \t var=" << sqrt(var) << endl;
	counter = 49;
      }
    }

    cout << "VideoConsumer::svc() exiting." << endl;
    return 0;
  }

  int Consumer::getImageSize() const
  {
    return connection.videoDevice.getImageSize();
  }

  int Consumer::getPaletteSize() const
  {
    switch (connection.videoDevice.getDevicePalette())
    {
    case paletteGrey:
      return 1;
      break;
			
    case paletteRGB:
    case paletteBGR:
      return 3;
      break;
			
    case paletteRGBA:
    case paletteABGR:
      return 4;
      break;
			
    default:
      throw Miro::Exception("can't get palette size: illegal image palette");
    }
  }

  Miro::TimeIDL
  Consumer::getCurrentImage(void * data)
  {
    Miro::Guard guard(mutex);

    if (!pCurrentImageData && !canceled())
      // we were too fast ;-)
    {
      ACE_Time_Value timeout(ACE_OS::gettimeofday());
      timeout += maxWait;
      if (cond.wait(&timeout) == -1)
	throw Miro::ETimeOut();
    }
    copyImageData(data, pCurrentImageData);
    return timeStamp_;
  }

  Miro::TimeIDL
  Consumer::getWaitNextImage(void * data)
  {
    Miro::Guard guard(mutex);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait;
    if (cond.wait(&timeout) == -1)
      throw Miro::ETimeOut();
    copyImageData(data, pCurrentImageData);
    return timeStamp_;
  }

  void
  Consumer::getWaitNextSubImage(unsigned char * dst, const int reqWidth, const int reqHeight)
  {
    Miro::Guard guard(mutex);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait;
    if (cond.wait(&timeout) == -1)
      throw Miro::ETimeOut();

    unsigned char * src = new unsigned char[getPaletteSize()
					    * connection.videoDevice.getImageWidth()
					    * connection.videoDevice.getImageHeight()];

    copyImageData(src, pCurrentImageData);
    shrinkImageData(dst, src, reqWidth, reqHeight);

    delete src;
  }

  void Consumer::shrinkImageData(unsigned char *dst, unsigned char *src, int reqWidth, int reqHeight)
  {
    int srcWidth  = connection.videoDevice.getImageWidth();
    int srcHeight = connection.videoDevice.getImageHeight();

    // do not expand
    reqWidth      = (reqWidth  > srcWidth)  ? srcWidth  : reqWidth;
    reqHeight     = (reqHeight > srcHeight) ? srcHeight : reqHeight;

    // src pixels per requested pixel
    double intervalWidth  = (double)srcWidth  / (double)reqWidth;
    double intervalHeight = (double)srcHeight / (double)reqHeight;

    const int paletteSize = getPaletteSize();
    unsigned long * tileValueSum = new unsigned long[paletteSize];

    for (int req_h = 0; req_h < reqHeight; ++req_h)
    {
      double low_h = (double)(req_h)     * intervalHeight;
      double up_h  = (double)(req_h + 1) * intervalHeight;
		
      for (int req_w = 0; req_w < reqWidth; ++req_w)
      {
	double low_w = (double)(req_w)     * intervalWidth;
	double up_w  = (double)(req_w + 1) * intervalWidth;

	long num_in_tile = 0;
	for (int palette = 0; palette < paletteSize; ++palette)
	  tileValueSum[palette] = 0;
			
	for (int src_h = (int)low_h; src_h < (int)up_h; ++src_h)
	{
	  for (int src_w = (int)low_w; src_w < (int)up_w; ++src_w)
	  {
	    for (int palette = 0; palette < paletteSize; ++palette)
	      tileValueSum[palette] += src[paletteSize * (src_h * srcWidth + src_w) + palette];
	    num_in_tile += 1;
	  }
	}

	for (int palette = 0; palette < paletteSize; ++palette)
	{
	  tileValueSum[palette] /= num_in_tile;
	  dst[paletteSize * (req_h * reqWidth + req_w) + palette] = (unsigned char)tileValueSum[palette];
	}
      }
    }

    delete tileValueSum;
  }

  void Consumer::copyImageData(void* dst, const void* src)
  {
    if (connection.videoDevice.getRequestedPalette() == connection.videoDevice.getDevicePalette())
    {
      switch (connection.videoDevice.getDevicePalette())
      {
      case paletteGrey:
	copy(dst, src, 1);
	break;

      case paletteRGB:
      case paletteBGR:
	copy(dst, src, 3);
	break;

      case paletteRGBA:
      case paletteABGR:
	copy(dst, src, 4);
	break;

      default:
	throw Miro::Exception("can't copy image: illegal image palette");
      }
    }
    else
    {
      switch (connection.videoDevice.getDevicePalette())
      {
      case paletteRGB:
	if (connection.videoDevice.getRequestedPalette() == paletteBGR)
	  swap3(dst, src);
	else
	  throw Miro::Exception("can't copy image: incompatible image palette");
	break;

      case paletteBGR:
	if (connection.videoDevice.getRequestedPalette() == paletteRGB)
	  swap3(dst, src);
	else
	  throw Miro::Exception("can't copy image: incompatible image palette");
	break;

      case paletteRGBA:
	if (connection.videoDevice.getRequestedPalette() == paletteABGR)
	  swap4(dst, src);
	else
	  throw Miro::Exception("can't copy image: incompatible image palette");
	break;

      case paletteABGR:
	if (connection.videoDevice.getRequestedPalette() == paletteRGBA)
	  swap4(dst, src);
	else
	  throw Miro::Exception("can't copy image: incompatible image palette");
	break;
      default:
	throw Miro::Exception("can't copy image: incompatible image palette");
      }
    }
  }

  void Consumer::copy(void* dst, const void* src, const int pixSize)
  {
    char*	start = (char*)src;
    char*	target = (char*)dst;
    int		h = connection.videoDevice.getImageHeight();
    int		w = connection.videoDevice.getImageWidth();
    int		bytesPerLine = w*pixSize;

    if ((connection.videoDevice.getRequestedSubfield() != subfieldAll) &&
	(connection.videoDevice.getDeviceSubfield() == subfieldAll))
    {
      int	srcOffset = 2*bytesPerLine;
      if (connection.videoDevice.getRequestedSubfield() == subfieldOdd)
	start += srcOffset;
      for (int i=0; i<h; i++)
      {
	memcpy(target, start, bytesPerLine);
	start += srcOffset;
	target += bytesPerLine;
      }
    }
    else
      memcpy(target, start, bytesPerLine * h);
  }

  void Consumer::swap3(void* dst, const void* src)
  {
    char*	start = (char*)src;
    char*	target = (char*)dst;
    int		h = connection.videoDevice.getImageHeight();
    int		w = connection.videoDevice.getImageWidth();
    int		srcOffset = w*3;
    int		targetOffset = srcOffset;

    if ((connection.videoDevice.getRequestedSubfield() != subfieldAll) &&
	(connection.videoDevice.getDeviceSubfield() == subfieldAll))
    {
      if (connection.videoDevice.getRequestedSubfield() == subfieldOdd)
	start += srcOffset;
      srcOffset *= 2;
    }
    for (int i=0; i<h; i++)
    {
      swapLine3(target, start, w);
      start += srcOffset;
      target += targetOffset;
    }
  }

  void Consumer::swap4(void* dst, const void* src)
  {
    char*	start = (char*)src;
    char*	target = (char*)dst;
    int		h = connection.videoDevice.getImageHeight();
    int		w = connection.videoDevice.getImageWidth();
    int		srcOffset = w*4;
    int		targetOffset = srcOffset;

    if ((connection.videoDevice.getRequestedSubfield() != subfieldAll) &&
	(connection.videoDevice.getDeviceSubfield() == subfieldAll))
    {
      if (connection.videoDevice.getRequestedSubfield() == subfieldOdd)
	start += srcOffset;
      srcOffset *= 2;
    }
    for (int i=0; i<h; i++)
    {
      swapLine4(target, start, w);
      start += srcOffset;
      target += targetOffset;
    }
  }

  void Consumer::swapLine3(void* dst, const void* src, const int n)
  {
    char*	start = (char*)src;
    char*	top = start + (3*n);
    char*	target = (char*)dst;
    char		r, g, b;

    while (start<top)
    {
      b = *start++;
      g = *start++;
      r = *start++;
      *target++ = r;
      *target++ = g;
      *target++ = b;
    }
  }

  void Consumer::swapLine4(void* dst, const void* src, const int n)
  {
    char*	start = (char*)src;
    char*	top = start + (4*n);
    char*	target = (char*)dst;
    char		r, g, b, a;

    while (start<top)
    {
      a = *start++;
      b = *start++;
      g = *start++;
      r = *start++;

      *target++ = r;
      *target++ = g;
      *target++ = b;
      *target++ = a;
    }
  }
};
