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
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <miro/Exception.h>
#include "VideoDevice.h"
#include "VideoDeviceBTTV.h"
//#include "VideoDeviceV4L.h"
#include "VideoDeviceMeteor.h"
//#include "VideoDevice1394.h"

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
  using std::cout;
  using std::endl;
  using std::string;

  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------

VideoDevice::VideoDevice(const string& dev)
	{
	DBG(cout << "Constructing VideoDevice '" << dev << "'" << endl);

	workerDevice = NULL;
	sourceID = 0;
	requestedPaletteID = 0;
	devicePaletteID = 0;
	imgWidth = 0;
	imgHeight = 0;

	if (dev == "bttv")
		workerDevice = new  VideoDeviceBTTV;
	//else if (dev == "v4l")
	//	workerDevice = new  VideoDeviceV4L;
	else if (dev == "meteor")
		workerDevice = new  VideoDeviceMeteor;
	//else if (dev == "1394")
	//	workerDevice = new  VideoDevice1394;
 	else
		throw Miro::Exception(string("VideoDevice::constructor: unknown device: ") + dev);
	}

VideoDevice::~VideoDevice()
	{
 	if (workerDevice)
		delete workerDevice;
	}



void VideoDevice::connect(const int fd, const int buffers, const string& fmt, const string& src,
						const string& pal, const string& sub, const int w, const int h)
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::connect: no worker device");
 	#endif // DEBUG
	int	iFormat;
	int	iSource;
	int	iPalette;
	int	iSubfield;

	if (fmt == "auto")
		iFormat = formatAuto;
	else if (fmt == "pal")
		iFormat = formatPAL;
	else if (fmt == "ntsc")
		iFormat = formatNTSC;
	else if (fmt == "secam")
		iFormat = formatSECAM;
	else
		throw Miro::Exception(string("VideoDevice::constructor: unknown format: ") + fmt);

	if (src == "composite1")
		iSource = sourceComposite1;
	else if (src == "composite2")
		iSource = sourceComposite2;
	else if (src == "composite3")
		iSource = sourceComposite3;
	else if (src == "svideo")
		iSource = sourceSVideo;
	else if (src == "tuner")
		iSource = sourceTuner;
	else if (src == "usb")
		iSource = sourceUSB;
	else if (src == "1394")
		iSource = source1394;
	else
		throw Miro::Exception(string("VideoDevice::constructor: unknown source: ") + src);


	if ((pal == "grey") || (pal == "gray"))
		iPalette = paletteGrey;
	else if (pal == "rgb")
		iPalette = paletteRGB;
	else if (pal == "rgba")
		iPalette = paletteRGBA;
	else if (pal == "bgr")
		iPalette = paletteBGR;
	else if (pal == "abgr")
		iPalette = paletteABGR;
	else
		throw Miro::Exception(string("VideoDevice::constructor: unknown palette: ") + pal);

	if (sub == "all")
		iSubfield = subfieldAll;
	else if (sub == "odd")
		iSubfield = subfieldOdd;
	else if (sub == "even")
		iSubfield = subfieldEven;
	else
		throw Miro::Exception(string("VideoDevice::constructor: unknown subfield: ") + sub);

	workerDevice->handleConnect(fd, buffers, iFormat, iSource, iPalette, iSubfield, w, h);
	}

void VideoDevice::disconnect()
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::disconnect: no worker device");
 	#endif // DEBUG
	workerDevice->handleDisconnect();
	}

int VideoDevice::countBuffers() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::countBuffers: no worker device");
 	#endif // DEBUG
 	return workerDevice->iNBuffers;
	}

int VideoDevice::countFramesCaptured() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::countFramesCaptured: no worker device");
 	#endif // DEBUG
 	return workerDevice->iNFramesCaptured;
	}

int VideoDevice::getFormat() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::getFormat: no worker device");
 	#endif // DEBUG
 	return workerDevice->formatID;
	}

int VideoDevice::getSource()  const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::getSource: no worker device");
 	#endif // DEBUG
	return workerDevice->sourceID;
	}

int VideoDevice::getDevicePalette() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::getDevicePalette: no worker device");
 	#endif // DEBUG
	return workerDevice->devicePaletteID;
	}

int VideoDevice::getRequestedPalette() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::getRequestedPalette: no worker device");
 	#endif // DEBUG
	return workerDevice->requestedPaletteID;
	}

int VideoDevice::getDeviceSubfield() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::getDeviceSubfield: no worker device");
 	#endif // DEBUG
	return workerDevice->deviceSubfieldID;
	}

int VideoDevice::getRequestedSubfield() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::getRequestedSubfield: no worker device");
 	#endif // DEBUG
	return workerDevice->requestedSubfieldID;
	}

 
int VideoDevice::getImageWidth() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::getImageWidth: no worker device");
 	#endif // DEBUG
 	return workerDevice->imgWidth;
	}

int VideoDevice::getImageHeight() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::getImageHeight: no worker device");
 	#endif // DEBUG
 	return workerDevice->imgHeight;
	}

int VideoDevice::getImageSize() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		cout << "VideoDevice::getImageSize: no worker device" << endl;
 	#endif // DEBUG
	return workerDevice->imgWidth * workerDevice->imgHeight * getPixelSize(workerDevice->requestedPaletteID);
	}

int VideoDevice::getDeviceImageSize() const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		cout << "VideoDevice::getImageSize: no worker device" << endl;
 	#endif // DEBUG
	return workerDevice->imgWidth * workerDevice->imgHeight * getPixelSize(workerDevice->devicePaletteID);
	}

void* VideoDevice::grabImage(ACE_Time_Value& _timeStamp) const
	{
 	#ifdef DEBUG
	if (!workerDevice)
		throw Miro::Exception("VideoDevice::grabImage: no worker device");
 	#endif // DEBUG
	return workerDevice->grabImage(_timeStamp);
	}

int VideoDevice::getPixelSize(const int pal) const
	{
	int	pixelSize = 0;

	switch (pal)
		{
		case paletteGrey:	//	gray
			pixelSize = 1;
			break;
		case paletteRGB:	//	rgb24:
		case paletteBGR:	//	bgr24:
			pixelSize = 3;
			break;
		case paletteRGBA:	//	rgb32:
		case paletteABGR:	//	bgr32:
			pixelSize = 4;
			break;
		default:
			cout << "VideoDevice::getImageSize: unknown palette" << endl;
			break;
		}
	return pixelSize;
	}

void VideoDevice::handleConnect(const int, const int, const int, const int, const int, const int, const int, const int)
	{
	throw Miro::Exception(string("VideoDevice::handleConnect: assume as 'pure virtual' ;-)"));
	}

void VideoDevice::handleDisconnect()
	{
	throw Miro::Exception(string("VideoDevice::handleDisconnect: assume as 'pure virtual' ;-)"));
	}

VideoDevice::VideoDevice()
	{
	workerDevice = NULL;
	formatID = 0;
	sourceID = 0;
	requestedPaletteID = 0;
	devicePaletteID = 0;
	requestedSubfieldID = 0;
	deviceSubfieldID = 0;
	imgWidth = 0;
	imgHeight = 0;
	}

};

