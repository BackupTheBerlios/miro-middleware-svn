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

#include "VideoDeviceBase.h"

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  class VideoDeviceBTTV : public VideoDeviceBase
  {
  public:
    VideoDeviceBTTV();
    virtual ~VideoDeviceBTTV();

    virtual void * grabImage(ACE_Time_Value& _timeStamp) const;

  protected:
    virtual void handleConnect(const int fd, const Parameters& params);
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
    char * map_;

    mutable int currentBuffer_;
    mutable int nextBuffer_;

    // static protected data
    //! screen grab
    static int gb_pal[64];
  };
};
#endif
