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
#ifndef VideoDeviceMeteor_h
#define VideoDeviceMeteor_h

// extern "C" {
#include "ioctl_meteor.h"
// }
#include "VideoDevice.h"

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  class VideoDeviceMeteor  :public VideoDevice
  {
  public:
    VideoDeviceMeteor();
    virtual	~VideoDeviceMeteor();

    virtual	void*	grabImage() const;

  protected:
    virtual	void	handleConnect(int fd, int buffers, int fmt, int src, int pal, int sub, int w, int h);
    virtual	void	handleDisconnect();

    virtual	void	setFormat(int);
    virtual	void	setSource(int);
    virtual	void	setPalette(int);
    virtual	void	setSize(int, int);

    int getCurrentErrorCount() const;

    meteor_geomet	meteorGeometry;

    int			videoFd;
    char*		map;
  };
};
#endif
