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
#include <ace/OS.h>

#include "VideoDeviceQuickCam.h"
#include "BufferManagerQuickCam.h"

#include "miro/Exception.h"

#include <iostream>
#include <string>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif


namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(DeviceQuickCam);


  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  int DeviceQuickCam::gb_pal[64];

  DeviceQuickCam::DeviceQuickCam(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    devName_(),
    ioBuffer_(),
    connector_(),
//    gb(NULL),
    channels(NULL),
    map_(NULL),
    currentBuffer_(0),
    nextBuffer_(0)
  {
    DBG(std::cout << "Constructing DeviceQuickCam." << std::endl);

    /**
       cleanup supported format lookup table
    */
    for (int i = 0; i < 64; ++i)
      gb_pal[i] = 0;

  }

  DeviceQuickCam::~DeviceQuickCam()
  {
//    delete gb;
  }


  BufferManager *
  DeviceQuickCam::bufferManagerInstance() const
  {
    unsigned char * bufferAddr[gb_buffers.frames];

    for (int i = 0; i < gb_buffers.frames; ++i)
      bufferAddr[i] = map_ + gb_buffers.offsets[i];

    return new BufferManagerQuickCam(this,
				 ioBuffer_.get_handle(),
				 outputFormat_.width,
				 outputFormat_.height,
				 15,
				 gb_buffers.frames,
				 bufferAddr);
  }


  void
  DeviceQuickCam::init(Miro::Server& _server, FilterParameters const * _params)
  {
    int	err = 0;

    DBG(std::cout << "Connecting DeviceQuickCam." << std::endl);

    params_ = dynamic_cast<DeviceQuickCamParameters const *>(_params);
    assert(params_ != NULL);

    devName_.set(params_->device.c_str());
    if (connector_.connect(ioBuffer_,
			  devName_,
			  0, ACE_Addr::sap_any, 0, O_RDONLY) == -1) {
      cerr << "Failed to open device: " << params_->device << endl
	   << "Propably running on the wrong machine?" << endl;
      throw Miro::CException(errno, std::strerror(errno));
    }

    //fcntl(ioBuffer_.get_handle(), F_SETFD, FD_CLOEXEC);
    pwc_probe probe;
    if(ioctl(ioBuffer_.get_handle(), VIDIOCPWCPROBE, &probe) == 0) {
	std::cout << "Probing Device: " << probe.name << " successfull" << endl;
    }
    else{
       throw Miro::CException(errno, "Error Probing Device QuickCam");
    }

    /* video capabilities */
    getCapabilities();

    /* video channels aka inputs */
    getChannels();

    /* set video channel */

    setChannel();


    if (capability.type & VID_TYPE_CAPTURE) {
      err = ioctl(ioBuffer_.get_handle(), VIDIOCGMBUF, &gb_buffers);
      if (err == -1)
	throw Miro::CException(errno, "DeviceQuickCam::handleConnect() - VIDIOCGMBUF");
      map_ = (unsigned char*)mmap(0, gb_buffers.size, PROT_READ, MAP_SHARED, ioBuffer_.get_handle(), 0);
      if ((int)map_ == -1)
	throw Miro::CException(errno, "DeviceQuickCam::handleConnect() - mmap()");
    }

    iNBuffers = gb_buffers.frames;

    std::cout << "buffersize: " << gb_buffers.size << std::endl;
    std::cout << "buffersize/frame: " << gb_buffers.size/gb_buffers.frames << std::endl;
    std::cout << "frames: " << gb_buffers.frames << std::endl;

    probeAllFormats();

    //setFormat();
    //setSource();
    setPalette();

    ioctl(ioBuffer_.get_handle(), VIDIOCGWIN, &vidwindow);

    std::cout << "fps " << (vidwindow.flags >> 16) << endl;

    vidwindow.flags = params_->fps << 16;

    std::cout << "setting " << params_->fps << " fps" << endl;
    ioctl(ioBuffer_.get_handle(), VIDIOCSWIN, &vidwindow);

    ioctl(ioBuffer_.get_handle(), VIDIOCGWIN, &vidwindow);

    std::cout << "fps now " << (vidwindow.flags >> 16) << endl;



    //	preparing buffers

/*    delete gb;
    gb = new struct video_mmap[gb_buffers.frames];

    for (int i = 0; i < gb_buffers.frames; ++i) {
      gb[i].width  = outputFormat_.width;
      gb[i].height = outputFormat_.height;
      gb[i].format = 15;
      gb[i].frame = i;
    }

    err = ioctl(ioBuffer_.get_handle(), VIDIOCMCAPTURE, &(gb[nextBuffer_]));
    ++nextBuffer_; */

    Super::init(_server, _params);

  }

  void DeviceQuickCam::fini()
  {
    DBG(std::cout << "DeviceQuickCam." << std::endl);

    delete[] channels;
    channels = NULL;

    if ((int)map_ != -1 && map_ != NULL) {
      munmap(map_, gb_buffers.size);
      map_ = NULL;
    }
    ioBuffer_.close();

    // removing buffer ptr
    //buffer_ = NULL;
  }



  void DeviceQuickCam::setPalette()
  {
    DBG(std::cout << "DeviceQuickCam: setPalette" << std::endl);

    outputFormat_.palette = Miro::YUV_420P;

  }



/*  void
  DeviceQuickCam::acquireOutputBuffer()
  {


    // synch current image
    int err = ioctl(ioBuffer_.get_handle(), VIDIOCSYNC, &currentBuffer_);
    if (err == -1) {
      throw Miro::CException(errno, "DeviceQuickCam::grabImage() - VIDIOCSYNC");
    }

    // set time stamp
    timeStamp_ = ACE_OS::gettimeofday();
    // set buffer
    buffer_ = map_ + gb_buffers.offsets[currentBuffer_];

    // update the follower buffer pointer
    ++currentBuffer_;
    if (currentBuffer_ == iNBuffers)
      currentBuffer_ = 0;

    // grab next image
    err = ioctl(ioBuffer_.get_handle(), VIDIOCMCAPTURE, &(gb[nextBuffer_]));
    if (err == -1) {
      throw Miro::CException(errno, "DeviceQuickCam::grabImage() - VIDIOCMCAPTURE");
    }

    // update the leader buffer pointer
    ++nextBuffer_;
    if (nextBuffer_ == iNBuffers)
      nextBuffer_ = 0;
  }*/

/*  void
  DeviceQuickCam::releaseOutputBuffer()
  {
  }*/

  void DeviceQuickCam::getCapabilities()
  {
    DBG(std::cout << "DeviceQuickCam: getCapabilities" << std::endl);

    int	err = ioctl(ioBuffer_.get_handle(), VIDIOCGCAP, &capability);
    if (err == -1)
      throw Miro::CException(errno, "DeviceQuickCam::getCapabilities() - VIDIOCAP");
  }


  void DeviceQuickCam::getChannels()
  {
    DBG(std::cout << "DeviceQuickCam: getChannels" << std::endl);

    /* input sources */
    channels = new struct video_channel[capability.channels];
    for (int i = 0; i < capability.channels; i++) {
      channels[i].channel = i;
      int err = ioctl(ioBuffer_.get_handle(), VIDIOCGCHAN, &channels[i]);
      if (err == -1)
	throw Miro::CException(errno, "DeviceQuickCam::getChannels() - VIDIOCGCHAN");
    }
  }

  void DeviceQuickCam::setChannel()
  {
    DBG(std::cout << "DeviceQuickCam: setChannel" << std::endl);

    int err = ioctl(ioBuffer_.get_handle(), VIDIOCSCHAN, &(channels[0]));

    if (err < 0)
       throw Miro::CException(errno, "DeviceQuickCam::setChannel() - VIDIOCSCHAN");


  }

  bool
  DeviceQuickCam::probeFormat(int format)
  {
    DBG(std::cout << "DeviceQuickCam: probeFormat" << std::endl);

    struct video_mmap gb;
    int	err;

    if (0 != gb_pal[format])
      goto done;

    gb.frame  = 0;
    gb.width  = 64;
    gb.height = 48;
    gb.format = format;

    err = ioctl(ioBuffer_.get_handle(), VIDIOCMCAPTURE, &gb);
    if (err == -1) {
      gb_pal[format] = 2;
      goto done;
    }

    err = ioctl(ioBuffer_.get_handle(), VIDIOCSYNC, &gb);
    if (err == -1) {
      gb_pal[format] = 3;
      goto done;
    }

    gb_pal[format] = 1;

  done:
    return gb_pal[format] == 1;
  }

  int
  DeviceQuickCam::probeAllFormats()
  {
    int	count = 0;
    for (int i = 0; i < 64; ++i) {
      if (probeFormat(i))
	++count;
    }
    return count;
  }



};

