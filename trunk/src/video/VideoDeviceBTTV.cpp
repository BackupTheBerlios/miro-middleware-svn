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

#include "VideoDeviceBTTV.h"

#include "miro/Exception.h"

#include <iostream>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  int VideoDeviceBTTV::gb_pal[64];

  VideoDeviceBTTV::VideoDeviceBTTV() :
    gb(NULL),
    channels(NULL),
    videoFd(-1),
    map_((char*)-1),
    currentBuffer_(0),
    nextBuffer_(0)
  {
    DBG(std::cout << "Constructing VideoDeviceBTTV." << std::endl);

    /**
       cleanup supported format lookup table
    */
    for (int i = 0; i < 64; ++i)
      gb_pal[i] = 0;

    formatLookup[formatAuto] = VIDEO_MODE_AUTO;
    formatLookup[formatPAL] = VIDEO_MODE_PAL;
    formatLookup[formatNTSC] = VIDEO_MODE_NTSC;
    formatLookup[formatSECAM] = VIDEO_MODE_SECAM;

    paletteLookup[paletteGrey] = VIDEO_PALETTE_GREY;
    paletteLookup[paletteRGB] = VIDEO_PALETTE_RGB24;
    paletteLookup[paletteRGBA] = VIDEO_PALETTE_RGB32;
    paletteLookup[paletteBGR] = VIDEO_PALETTE_RGB24;
    paletteLookup[paletteABGR] = VIDEO_PALETTE_RGB32;

    sourceLookup[sourceComposite1] = 1;
    sourceLookup[sourceComposite2] = 3;
    sourceLookup[sourceComposite3] = -1;
    sourceLookup[sourceSVideo] = 2;
    sourceLookup[sourceTuner] = 0;
    sourceLookup[sourceUSB] = -1;
    sourceLookup[source1394] = -1;
  }

  VideoDeviceBTTV::~VideoDeviceBTTV()
  {
    handleDisconnect();
    delete gb;
  }


  void VideoDeviceBTTV::handleConnect(int fd, int, int fmt, int src, int pal, int sub, int w, int h)
  {
    int	err = 0;

    DBG(std::cout << "Connecting VideoDeviceBTTV." << std::endl);

    videoFd = fd;

    fcntl(videoFd, F_SETFD, FD_CLOEXEC);
    /* video capabilities */
    getCapabilities();
    /* video channels aka inputs */
    getChannels();

    if (capability.type & VID_TYPE_CAPTURE) {
      err = ioctl(videoFd, VIDIOCGMBUF, &gb_buffers);
      if (err == -1)
	throw Miro::CException(errno, "VideoDeviceBTTV::handleConnect() - VIDIOCGMBUF");

      map_ = (char*)mmap(0,gb_buffers.size,PROT_READ,MAP_SHARED,videoFd,0);
      if (map_ == (char *)-1)
	throw Miro::CException(errno, "VideoDeviceBTTV::handleConnect() - mmap()");
    }

    iNBuffers = gb_buffers.frames;

    std::cout << "buffersize: " << gb_buffers.size << std::endl;
    std::cout << "buffersize/frame: " << gb_buffers.size/gb_buffers.frames << std::endl;
    std::cout << "frames: " << gb_buffers.frames << std::endl;

    probeAllFormats();

    setFormat(fmt);
    setSource(src);
    setPalette(pal);
    setSize(w, h);

    iNFramesCaptured = 0;

    //	preparing buffers

    delete gb;
    gb = new struct video_mmap[gb_buffers.frames];

    for (int i = 0; i < gb_buffers.frames; ++i) {
      gb[i].width  = imgWidth;
      gb[i].height = imgHeight;
      gb[i].format = paletteLookup[devicePaletteID];
      gb[i].frame = i;
    }

    requestedSubfieldID = sub;

    if (requestedSubfieldID != subfieldAll) {
      for (int i = 0; i < gb_buffers.frames; ++i) {
	gb[i].height = 2*imgHeight;
      }

      if (capability.type & VID_TYPE_SUBCAPTURE) {

	struct video_capture vc;

	vc.x = 0;
	vc.y = 0;
	vc.width = gb[0].width;
	vc.height = gb[0].height;
	vc.decimation = 1;
	vc.flags = (requestedSubfieldID == subfieldOdd)? VIDEO_CAPTURE_ODD : VIDEO_CAPTURE_EVEN;

	err = ioctl(videoFd, VIDIOCSCAPTURE, &vc);
	if (err == -1)
	  throw Miro::CException(errno, "VideoDeviceBTTV::handleConnect() - VIDIOCGMBUF");

	deviceSubfieldID = requestedSubfieldID;
      }
     else {
	std::cout << "Warning: hardware doesn't support subfields ;-(" << std::endl;
	deviceSubfieldID = subfieldAll;
      }
    }

    err = ioctl(videoFd, VIDIOCMCAPTURE, &(gb[nextBuffer_]));
    ++nextBuffer_;
    if (err != -1 && iNBuffers > 2) {
      err = ioctl(videoFd, VIDIOCMCAPTURE, &(gb[nextBuffer_]));
      ++nextBuffer_;
    }
    if (err == -1)
      throw Miro::CException(errno, "VideoDeviceBTTV::handleConnect() - VIDIOCMCAPTURE");
  }

  void VideoDeviceBTTV::handleDisconnect()
  {
    DBG(std::cout << "VideoDeviceBTTV: frames captured " << iNFramesCaptured << std::endl);

    delete[] channels;
    channels = NULL;

    if (map_ != (char*)-1) {
      munmap(map_, gb_buffers.size);
      map_ = (char*)-1;
    }
    videoFd = -1;
  }


  void VideoDeviceBTTV::setFormat(int id)
  {
    DBG(std::cout << "VideoDeviceBTTV: setFormat" << std::endl);

    if (formatLookup[id] != -1)
      formatID = id;
    else
      throw Miro::Exception("VideoDeviceBTTV::setFormat");
  }

  void VideoDeviceBTTV::setSource(int id)
  {
    DBG(std::cout << "VideoDeviceBTTV: setSource" << std::endl);

    if ((sourceLookup[id] != -1) && (sourceLookup[id] < capability.channels)) {
      sourceID = id;
      int err = ioctl(videoFd, VIDIOCSCHAN, &channels[sourceLookup[sourceID]]);
      if (err == -1)
	throw Miro::CException(errno, "VideoDeviceBTTV::setSource() - VIDIOCSCHAN");
    }
    else
      throw Miro::Exception("VideoDeviceBTTV::setSource");
  }

  void VideoDeviceBTTV::setPalette(int id)
  {
    DBG(std::cout << "VideoDeviceBTTV: setPalette" << std::endl);

    requestedPaletteID = id;

    if (id == paletteRGB)
      id =  paletteBGR;
    else if (id == paletteRGBA)
      id =  paletteABGR;

    if (gb_pal[paletteLookup[id]] == 1)
      devicePaletteID = id;
    else
      throw Miro::Exception("VideoDeviceBTTV::setPalette");
  }

  void VideoDeviceBTTV::setSize(int w, int h)
  {
    DBG(std::cout << "VideoDeviceBTTV: setSize" << std::endl);

    if ((w>=capability.minwidth) && (w <= capability.maxwidth))
      imgWidth = w;
    else
      throw Miro::Exception("VideoDeviceBTTV::setSize: illegal width");

    if ((h >= capability.minheight) && (h <= capability.maxheight))
      imgHeight = h;
    else
      throw Miro::Exception("VideoDeviceBTTV::setSize: illegal height");
  }

  void* VideoDeviceBTTV::grabImage(ACE_Time_Value& _timeStamp) const
  {
    DBG(std::cout << "VideoDeviceBTTV: grabImage" << std::endl);

    // synch current image
    int err = ioctl(videoFd, VIDIOCSYNC, &currentBuffer_);
    if (err == -1) {
      throw Miro::CException(errno, "VideoDeviceBTTV::grabImage() - VIDIOCSYNC");
    }
    // set time stamp
    _timeStamp = ACE_OS::gettimeofday();

    void * buffer = map_ + gb_buffers.offsets[currentBuffer_];

    // update the follower buffer pointer
    ++currentBuffer_;
    if (currentBuffer_ == iNBuffers)
      currentBuffer_ = 0;

    // grab next image
    err = ioctl(videoFd, VIDIOCMCAPTURE, &(gb[nextBuffer_]));
    if (err == -1) {
      throw Miro::CException(errno, "VideoDeviceBTTV::grabImage() - VIDIOCMCAPTURE");
    }

    // update the leader buffer pointer
    ++nextBuffer_;
    if (nextBuffer_ == iNBuffers)
      nextBuffer_ = 0;
    
    // runtime statistics
    ++iNFramesCaptured;

    return buffer;
  }

  void VideoDeviceBTTV::getCapabilities()
  {
    DBG(std::cout << "VideoDeviceBTTV: getCapabilities" << std::endl);

    int	err = ioctl(videoFd, VIDIOCGCAP, &capability);
    if (err == -1)
      throw Miro::CException(errno, "VideoDeviceBTTV::getCapabilities() - VIDIOCAP");
  }

  void VideoDeviceBTTV::getChannels()
  {
    DBG(std::cout << "VideoDeviceBTTV: getChannels" << std::endl);

    int	i;
    int	err;

    /* input sources */
    channels = new struct video_channel[capability.channels];
    for (i = 0; i < capability.channels; i++) {
      channels[i].channel = i;
      err = ioctl(videoFd, VIDIOCGCHAN, &channels[i]);
      if (err == -1)
	throw Miro::CException(errno, "VideoDeviceBTTV::getChannels() - VIDIOCGCHAN");
    }
  }


  bool
  VideoDeviceBTTV::probeFormat(int format)
  {
    DBG(std::cout << "VideoDeviceBTTV: probeFormat" << std::endl);

    struct video_mmap gb;
    int	err;

    if (0 != gb_pal[format])
      goto done;

    gb.frame  = 0;
    gb.width  = 64;
    gb.height = 48;
    gb.format = format;

    err = ioctl(videoFd, VIDIOCMCAPTURE, &gb);
    if (err == -1) {
      gb_pal[format] = 2;
      goto done;
    }

    err = ioctl(videoFd, VIDIOCSYNC, &gb);
    if (err == -1) {
      gb_pal[format] = 3;
      goto done;
    }

    gb_pal[format] = 1;

  done:
    return gb_pal[format] == 1;
  }

  int 
  VideoDeviceBTTV::probeAllFormats()
  {
    int	count = 0;

    for (int i = 0; i < 64; ++i) {
      if (probeFormat(i))
	++count;
    }
    return count;
  }
};

