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
#ifndef VideoDeviceBTTV_h
#define VideoDeviceBTTV_h

extern "C" {
#include <linux/videodev.h>
}

#include "VideoDevice.h"

namespace Video
{
//--------------------------------------------------------------------------
// Hardware specifica
//--------------------------------------------------------------------------
class VideoDeviceBTTV :public VideoDevice
	{
	public:
		VideoDeviceBTTV();
		virtual	~VideoDeviceBTTV();

		virtual	void*	grabImage() const;

	protected:
		virtual	void	handleConnect(const int fd, const int buffers, const int fmt, const int src, const int pal, const int sub, const int w, const int h);
		virtual	void	handleDisconnect();

		virtual	void	setFormat(int);
		virtual	void	setSource(int);
		virtual	void	setPalette(int);
		virtual	void	setSize(int, int);

		virtual	void	getCapabilities();
		virtual	void	getChannels();

		virtual	bool	probeFormat(int format);
		virtual	int		probeAllFormats();

		mutable struct video_mmap 		gb;
		struct video_mbuf        	gb_buffers;
		struct video_capability	capability;
		struct video_channel*		channels;
		//	struct video_buffer		ov_fbuf;
		//	struct video_picture		pict;

		int			videoFd;
		char*		map;
  };
};
#endif
