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

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <miro/Exception.h>
#include "VideoDevice.h"
#include "VideoDeviceBase.h"
#include "VideoDeviceBTTV.h"
//#include "VideoDeviceV4L.h"
#include "VideoDeviceMeteor.h"
#include "VideoDevice1394.h"
#include "VideoDeviceDummy.h"

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#define DBG_CHECK_POINTER(p,t) if (!p) throw Miro::Exception(t);
#else
#define DBG(x)
#define DBG_CHECK_POINTER(p,t)
#endif

namespace Video
{
    using std::cout;
    using std::endl;
    using std::string;

  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------

//--------------------------------------------------------------------
  VideoDevice::VideoDevice(const string& dev) :
    workerDevice(0)
  {
    DBG(cout << "Constructing VideoDevice '" << dev << "'" << endl);
    
    if (dev == "bttv")
      workerDevice = new  VideoDeviceBTTV;
    //else if (dev == "v4l")
    //	workerDevice = new  VideoDeviceV4L;
    else if (dev == "meteor")
      workerDevice = new  VideoDeviceMeteor;
#ifdef HAVE_VIDEODEVICE1394
    else if (dev == "1394")
      workerDevice = new  VideoDevice1394;
#endif // HAVE_VIDEODEVICE1394
    else if (dev == "dummy")
      workerDevice = new  VideoDeviceDummy;
    else
      throw Miro::Exception(string("VideoDevice::constructor: unknown device: ") + dev);
  }

//--------------------------------------------------------------------
    VideoDevice::~VideoDevice()
    {
 	if (workerDevice)
	    delete workerDevice;
    }

//--------------------------------------------------------------------
    void VideoDevice::connect()
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::connect: no worker device");
	workerDevice->handleConnect();
    }

//--------------------------------------------------------------------
    void VideoDevice::disconnect()
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::disconnect: no worker device");
	workerDevice->handleDisconnect();
    }

//--------------------------------------------------------------------
    int VideoDevice::countBuffers() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::countBuffers: no worker device");
 	return workerDevice->countBuffers();
    }

//--------------------------------------------------------------------
    int VideoDevice::countFramesCaptured() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::countFramesCaptured: no worker device");
 	return workerDevice->countFramesCaptured();
    }

//--------------------------------------------------------------------
    int VideoDevice::getFormat() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getFormat: no worker device");
 	return workerDevice->getFormat();
    }

//--------------------------------------------------------------------
    int VideoDevice::getSource()  const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getSource: no worker device");
	return workerDevice->getSource();
    }

//--------------------------------------------------------------------
    int VideoDevice::getDevicePalette() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getDevicePalette: no worker device");
	return workerDevice->getDevicePalette();
    }

//--------------------------------------------------------------------
    int VideoDevice::getRequestedPalette() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getRequestedPalette: no worker device");
	return workerDevice->getRequestedPalette();
    }

//--------------------------------------------------------------------
    int VideoDevice::getDeviceSubfield() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getDeviceSubfield: no worker device");
	return workerDevice->getDeviceSubfield();
    }

//--------------------------------------------------------------------
    int VideoDevice::getRequestedSubfield() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getRequestedSubfield: no worker device");
	return workerDevice->getRequestedSubfield();
    }

 //--------------------------------------------------------------------
    int VideoDevice::getImageWidth() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getImageWidth: no worker device");
 	return workerDevice->getImageWidth();
    }

//--------------------------------------------------------------------
    int VideoDevice::getImageHeight() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getImageHeight: no worker device");
 	return workerDevice->getImageHeight();
    }

//--------------------------------------------------------------------
    int VideoDevice::getImageSize() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getImageSize: no worker device");
	return workerDevice->getImageSize();
    }

//--------------------------------------------------------------------
    int VideoDevice::getDeviceImageSize() const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::getImageSize: no worker device");
	return workerDevice->getDeviceImageSize();
    }

//--------------------------------------------------------------------
    void* VideoDevice::grabImage(ACE_Time_Value & _timeStamp) const
    {
        DBG_CHECK_POINTER(workerDevice, "VideoDevice::grabImage: no worker device");
	return workerDevice->grabImage(_timeStamp);
    }
};

