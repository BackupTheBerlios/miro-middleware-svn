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


#include "VideoConsumer.h"
#include "VideoConnection.h"
#include "VideoDevice.h"
#include "VideoImpl.h"

#include <miro/Exception.h>
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
		     VideoImpl * _pGrabber) :
    connection(_connection),
    pGrabber(_pGrabber),
    mutex(),
    cond(mutex),
    running(false),
    pCurrentImageData(NULL)

   {
    DBG(cout << "Constructing VideoConsumer." << endl);
   }


  //----------------------//
  //----- destructor -----//
  //----------------------//
  Consumer::~Consumer()
  { 
    DBG(cout << "Destructing VideoConsumer." << endl);

  }

  int
  Consumer::open(void *)
  {
    running = true;
    return Super::open(0);
  };
  
  void
  Consumer::cancel()
  {
    cout << "VideoConsumer::cancel" << endl;
  
  running = false;

    // Wait for the thread of this Task to exit.
    // It is rather rude to let the Task go out of scope without doing 
    // this first.
    wait();

    DBG(cout << "VideoConsumer Task ended." << endl);
  }

  int 
  Consumer::svc()
  {
    cout << "Image size:" << getImageSize() << endl;
    pCurrentImageData = NULL;

    while (running)
    {
      try {
	void*	pNextImageData;
	pNextImageData = connection.videoDevice.grabImage();
	// lock();
	pCurrentImageData = pNextImageData;
	// release();
	cond.broadcast();
      }
      catch(Miro::CException& e) {
	cout << "VideoConsumer::svc() caught Miro::CException: " << endl
	     << e << endl;
      }
      catch(Miro::Exception& e) {
	cout << "VideoConsumer::svc() caught Miro::Exception: " << e << endl;
      }
    }

    cout << "VideoConsumer::svc() exiting." << endl;
    return 0;
  }

int Consumer::getImageSize() const
	{
	return connection.videoDevice.getImageSize();
	}

void Consumer::getCurrentImage(void* data)
	{
	Miro::Guard guard(mutex);

	if (!pCurrentImageData && running)
	// we where too fast ;-)
		{
		ACE_Time_Value	timeout(ACE_OS::gettimeofday());
		timeout += maxWait;
		if (cond.wait(&timeout) == -1)
			throw Miro::ETimeOut();
		}
	copyImageData(data, pCurrentImageData);
	}

void Consumer::getWaitNextImage(void* data)
	{
	Miro::Guard guard(mutex);
	ACE_Time_Value	timeout(ACE_OS::gettimeofday());
	timeout += maxWait;
	if (cond.wait(&timeout) == -1)
		throw Miro::ETimeOut();
	copyImageData(data, pCurrentImageData);
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
