// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
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

#include <ace/Mem_Map.h>

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  class VideoDeviceBTTV : public VideoDevice
  {
  public:
    VideoDeviceBTTV();
    virtual ~VideoDeviceBTTV();

    virtual void * grabImage() const;

  protected:
    virtual void handleConnect(int fd, int buffers, int fmt, int src, int pal, int sub, int w, int h);
    virtual void handleDisconnect();

    virtual void setFormat(int);
    virtual void setSource(int);
    virtual void setPalette(int);
    virtual void setSize(int, int);

    virtual void getCapabilities();
    virtual void getChannels();

    virtual bool probeFormat(int format);
    virtual int probeAllFormats();

    // protected data

    mutable struct video_mmap * gb;
    struct video_mbuf gb_buffers;
    struct video_capability capability;
    struct video_channel * channels;
    //	struct video_buffer		ov_fbuf;
    //	struct video_picture		pict;

    int videoFd;
    ACE_Mem_Map mmap_;
    char * map_;

    mutable int currentBuffer_;
    mutable int nextBuffer_;

    // static protected data
    //! screen grab
    static int gb_pal[64];
  };
};
#endif
