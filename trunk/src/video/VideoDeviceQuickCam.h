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
#ifndef VideoDeviceQuickCam_h
#define VideoDeviceQuickCam_h

extern "C" {
#include <linux/videodev.h>
#include <pwc-8.11/2.4.21/pwc-ioctl.h>
}


#include "VideoDevice.h"
#include <ace/DEV_Connector.h>

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  class DeviceQuickCam : public Device
  {
    typedef Device Super;
  public:

    DeviceQuickCam(const Miro::ImageFormatIDL& _inputFormat);
    virtual ~DeviceQuickCam();

    FILTER_PARAMETERS_FACTORY(DeviceQuickCam);

//    virtual void acquireOutputBuffer();
//    virtual void releaseOutputBuffer();

  protected:

    virtual BufferManager * bufferManagerInstance() const;
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void fini();

    // methods
    //void setFormat();
    //void setSource();
    void setPalette();
    void setSize();

    void getCapabilities();
    void getChannels();
    void setChannel();

    bool probeFormat(int format);
    int probeAllFormats();

    // static methods


    // data
    DeviceQuickCamParameters const * params_;

    ACE_DEV_Addr devName_;
    ACE_DEV_IO ioBuffer_;
    ACE_DEV_Connector connector_;

//    mutable struct video_mmap * gb;
    struct video_mbuf gb_buffers;
    struct video_capability capability;
    struct video_channel * channels;
    struct video_window vidwindow;

    unsigned char * map_;

    int		iNBuffers;
    mutable int	iCurrentBuffer;

    mutable int currentBuffer_;
    mutable int nextBuffer_;

    // static protected data
    //! screen grab
    static int gb_pal[64];
  };
};
#endif
