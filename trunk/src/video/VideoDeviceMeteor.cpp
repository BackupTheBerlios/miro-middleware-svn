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
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <miro/Exception.h>
#include <ace/OS.h>

#include "VideoDeviceMeteor.h"
#include "VideoConfig.h"

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

  VideoDeviceMeteor::VideoDeviceMeteor()
  {
    DBG(cout << "Constructing VideoDeviceMeteor '" << dev << "'" << endl);

    formatLookup[formatAuto] = METEOR_FMT_AUTOMODE;
    formatLookup[formatPAL] = METEOR_FMT_PAL;
    formatLookup[formatNTSC] = METEOR_FMT_NTSC;
    formatLookup[formatSECAM] = METEOR_FMT_SECAM;

    paletteLookup[paletteGrey] = METEOR_GEO_YUV_PLANAR;
    paletteLookup[paletteYUV] = -1;
    paletteLookup[paletteRGB] = METEOR_GEO_RGB24;
    paletteLookup[paletteRGBA] = METEOR_GEO_RGB24;
    paletteLookup[paletteBGR] = METEOR_GEO_RGB24;
    paletteLookup[paletteABGR] = METEOR_GEO_RGB24;

    sourceLookup[sourceComposite1] = METEOR_INPUT_DEV1;
    sourceLookup[sourceComposite2] = METEOR_INPUT_DEV2;
    sourceLookup[sourceComposite3] = METEOR_INPUT_DEV3;
    sourceLookup[sourceSVideo] = METEOR_INPUT_DEV_SVIDEO;
    sourceLookup[sourceTuner] = -1;
    sourceLookup[sourceUSB] = -1;
    sourceLookup[source1394] = -1;
  }

  VideoDeviceMeteor::~VideoDeviceMeteor()
  {
    handleDisconnect();
  }

  void VideoDeviceMeteor::handleConnect(int fd, const Parameters& params)
  {
    DBG(cout << "Connecting VideoDeviceMeteor." << endl);

    videoFd = fd;

    fcntl(videoFd,F_SETFD, FD_CLOEXEC);

    setPalette(getPalette(params.palette));
    setSize(params.width, params.height);

    meteorGeometry.frames = 1;
    if (::ioctl(videoFd, METEORSETGEO, &meteorGeometry) < 0)
      throw Miro::Exception("METEORSETGEO");

    setSource(Video::getSource(params.source));
    setFormat(Video::getFormat(params.format));

    map = (char*)mmap(0,getDeviceImageSize(),PROT_READ,MAP_SHARED,videoFd,0);
    if (map == (char*)-1)
      throw Miro::Exception("mmap()");

    iNFramesCaptured = 0;
  }

  void VideoDeviceMeteor::handleDisconnect()
  {
    DBG(cout << "VideoDeviceBTTV: frames captured " << iNFramesCaptured << endl);

    if (map != (char*)-1)
    {
      munmap(map,getDeviceImageSize());
      map = (char*)-1;
    }
    videoFd = -1;
  }


  void VideoDeviceMeteor::setFormat(int id)
  {
    if (formatLookup[id] != -1)
    {
      formatID = id;
      if (::ioctl(videoFd, METEORSFMT, &formatLookup[id]) < 0)
	throw Miro::Exception("METEORSFMT");
    }
    else
      throw Miro::Exception();
  }

  void VideoDeviceMeteor::setSource(int id)
  {
    DBG(cout << "VideoDeviceMeteor: setSource" << endl);

    sourceID = id;
    int	err = ioctl(videoFd, METEORSINPUT, &sourceLookup[sourceID]);
    if (err == -1)
      throw Miro::Exception("METEORSINPUT");
  }

  void VideoDeviceMeteor::setPalette(int id)
  {
    requestedPaletteID = id;
    if (id == paletteRGB)
      id =  paletteRGBA;
    devicePaletteID = paletteLookup[id];
    meteorGeometry.oformat = METEOR_GEO_YUV_PLANAR;
  }

  void VideoDeviceMeteor::setSize(int w, int h)
  {
    imgWidth = w;
    imgHeight = h;
    meteorGeometry.rows =  h;
    meteorGeometry.columns = w;
  }

  void* VideoDeviceMeteor::grabImage(ACE_Time_Value& _timeStamp) const
  {
    int		iNTries = 0;
    int		iNCaptureRetries = 16;
    bool	done = false;
    //	capture one frame
#ifdef DEBUG
    std::cout << "capture one frame" << std::endl;
#endif

    int	command = METEOR_CAP_SINGLE;
    int	errorCount = getCurrentErrorCount();

    if (::ioctl(videoFd, METEORCAPTUR, &command) < 0)
      throw Miro::Exception("METEORCAPTUR");

    while (!done && iNCaptureRetries && (iNTries < iNCaptureRetries))
    {
      int	currentErrorCount = getCurrentErrorCount();
      if (currentErrorCount == errorCount)
	done = true;
      else
      {
	iNTries++;
	errorCount = currentErrorCount;
	command = METEOR_CAP_SINGLE;
	if (ioctl(videoFd, METEORCAPTUR, &command) < 0)
	  throw Miro::Exception("METEORCAPTUR");
      }
    }

    _timeStamp = ACE_OS::gettimeofday();

    if (!done)
      throw Miro::Exception("VideoDeviceMeteor::grabImage");
    iNFramesCaptured++;
    return map;
  }

  int VideoDeviceMeteor::getCurrentErrorCount() const
  {
    struct meteor_counts	cnts;
    if (::ioctl(videoFd, METEORGCOUNT, &cnts) < 0)
      throw Miro::Exception("METEORGCOUNT");
    return cnts.fifo_errors + cnts.dma_errors;
  }

};

