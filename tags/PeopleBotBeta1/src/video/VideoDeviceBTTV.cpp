// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#include <ace/OS.h>

#include "VideoDeviceBTTV.h"

#include <miro/Exception.h>

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
/* screen grab */
static int                      gb_pal[64];

VideoDeviceBTTV::VideoDeviceBTTV()
	{
    DBG(std::cout << "Constructing VideoDeviceBTTV." << std::endl);

	videoFd = -1;
	map = (char*)-1;
	/**
		cleanup supported format lookup table
	*/
	for (int i=0; i<64; i++)
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
	}


void VideoDeviceBTTV::handleConnect(const int fd, const int, const int fmt, const int src, const int pal, const int sub, const int w, const int h)
	{
	int	err;

	DBG(std::cout << "Connecting VideoDeviceBTTV." << std::endl);

	videoFd = fd;

	fcntl(videoFd,F_SETFD,FD_CLOEXEC);
	/* video capabilities */
	getCapabilities();
	/* video channels aka inputs */
	getChannels();

	if (capability.type & VID_TYPE_CAPTURE)
		{
 		err = ioctl(videoFd,VIDIOCGMBUF,&gb_buffers);
		if (err == -1)
			throw Miro::CException(errno, "VideoDeviceBTTV::handleConnect() - VIDIOCGMBUF");
		map = (char*)mmap(0,gb_buffers.size,PROT_READ,MAP_SHARED,videoFd,0);
		if (map == (char*)-1)
			throw Miro::CException(errno, "VideoDeviceBTTV::handleConnect() - mmap()");
   		}

	iNBuffers = gb_buffers.frames;
	iCurrentBuffer = iNBuffers-1;

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

	gb.width  = imgWidth;
	gb.height = imgHeight;
	gb.format = paletteLookup[devicePaletteID];

	requestedSubfieldID = sub;

	if (requestedSubfieldID != subfieldAll)
		{
		gb.height = 2*imgHeight;
		if (capability.type & VID_TYPE_SUBCAPTURE)
			{
			struct video_capture	vc;

			vc.x = 0;
			vc.y = 0;
			vc.width = gb.width;
			vc.height = gb.height;
			vc.decimation = 1;
			vc.flags = (requestedSubfieldID == subfieldOdd)?VIDEO_CAPTURE_ODD:VIDEO_CAPTURE_EVEN;
			err = ioctl(videoFd,VIDIOCSCAPTURE,&vc);
			if (err == -1)
				throw Miro::CException(errno, "VideoDeviceBTTV::handleConnect() - VIDIOCGMBUF");
			deviceSubfieldID = requestedSubfieldID;
			}
		else
			{
			std::cout << "Warning: hardware doesn't support subfields ;-(" << std::endl;
			deviceSubfieldID = subfieldAll;
			}
		}

	gb.frame  = iCurrentBuffer;

	err = ioctl(videoFd,VIDIOCMCAPTURE,&gb);
	if (err == -1)
		throw Miro::CException(errno, "VideoDeviceBTTV::handleConnect() - VIDIOCMCAPTURE");
	}

void VideoDeviceBTTV::handleDisconnect()
	{
	DBG(std::cout << "VideoDeviceBTTV: frames captured " << iNFramesCaptured << std::endl);

	if (channels != NULL)
		{
		delete[] channels;
		channels = NULL;
		}
	if (map != (char*)-1)
		{
		munmap(map,gb_buffers.size);
		map = (char*)-1;
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

	if ((sourceLookup[id] != -1) && (sourceLookup[id] < capability.channels))
		{
		sourceID = id;
		int	err = ioctl(videoFd,VIDIOCSCHAN, &channels[sourceLookup[sourceID]]);
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

	if ((w>=capability.minwidth) && (w<=capability.maxwidth))
		imgWidth = w;
	else
		throw Miro::Exception("VideoDeviceBTTV::setSize: illegal width");
	if ((h>=capability.minheight) && (h<=capability.maxheight))
		imgHeight = h;
	else
		throw Miro::Exception("VideoDeviceBTTV::setSize: illegal height");
	}

  static int counter = 0;
  static int msec = 0;
  static int msec2 = 0;

void* VideoDeviceBTTV::grabImage() const
	{
	DBG(std::cout << "VideoDeviceBTTV: grabImage" << std::endl);

	ACE_Time_Value beginTime = ACE_OS::gettimeofday();

	int		err;
 	int		nextBuffer = (iCurrentBuffer+1)%iNBuffers;

        err = ioctl(videoFd,VIDIOCSYNC,&iCurrentBuffer);
        if (err == -1)
                throw Miro::CException(errno, "VideoDeviceBTTV::grabImage() - VIDIOCSYNC");

	ACE_Time_Value endTime = ACE_OS::gettimeofday();
	msec2 += (endTime - beginTime).msec();

	gb.frame  = nextBuffer;

	err = ioctl(videoFd,VIDIOCMCAPTURE,&gb);
	if (err == -1)
		throw Miro::CException(errno, "VideoDeviceBTTV::grabImage() - VIDIOCMCAPTURE");

	endTime = ACE_OS::gettimeofday();
	msec += (endTime - beginTime).msec();

	++counter %= 100;
	if (!counter) {
	  std::cout << "time for SYNC: " << msec2 / 100 << "msec" << std::endl;
	  std::cout << "time for grabbing: " << msec / 100 << "msec" << std::endl;
	  msec = msec2 = 0;
	}
	

	iNFramesCaptured++;

	//	std::cout << "grabbing buffer #" << gb.frame << std::endl;
	iCurrentBuffer = nextBuffer;
	return map + gb_buffers.offsets[iCurrentBuffer];
	}

void VideoDeviceBTTV::getCapabilities()
	{
	DBG(std::cout << "VideoDeviceBTTV: getCapabilities" << std::endl);

	int	err;

	err = ioctl(videoFd, VIDIOCGCAP, &capability);
	if (err == -1)
		throw Miro::CException(errno, "VideoDeviceBTTV::getCapabilities() - VIDIOCAP");
	}

void VideoDeviceBTTV::getChannels()
	{
	DBG(std::cout << "VideoDeviceBTTV: getChannels" << std::endl);

	int		i;
	int		err;

	 /* input sources */
	channels = new struct video_channel[capability.channels];
	for (i = 0; i < capability.channels; i++)
		{
		channels[i].channel = i;
		err = ioctl(videoFd,VIDIOCGCHAN,&channels[i]);
		if (err == -1)
			throw Miro::CException(errno, "VideoDeviceBTTV::getChannels() - VIDIOCGCHAN");
		}
	}


bool VideoDeviceBTTV::probeFormat(int format)
	{
    DBG(std::cout << "VideoDeviceBTTV: probeFormat" << std::endl);

	struct	video_mmap gb;
	int		err;

	if (0 != gb_pal[format])
		goto done;

	gb.frame  = 0;
	gb.width  = 64;
	gb.height = 48;

	gb.format = format;

	err = ioctl(videoFd,VIDIOCMCAPTURE,&gb);
	if (err == -1)
		{
		gb_pal[format] = 2;
		goto done;
		}

	err = ioctl(videoFd,VIDIOCSYNC,&gb);
	if (err == -1)
		{
		gb_pal[format] = 3;
		goto done;
		}

	gb_pal[format] = 1;

	done:
	return gb_pal[format] == 1;
	}

int VideoDeviceBTTV::probeAllFormats()
	{
	int	count = 0;
	int	i;

	for (i=0; i<64; i++)
		{
		if (probeFormat(i))
			count++;
		}
	return count;
	}
};

