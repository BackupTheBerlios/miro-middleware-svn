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
#ifndef VideoDeviceMeteor_h
#define VideoDeviceMeteor_h

// extern "C" {
#include "ioctl_meteor.h"
// }
#include "VideoDevice.h"

#include <ace/DEV_Connector.h>

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  class VideoDeviceMeteor  :public VideoDevice
  {
    typedef VideoDevice Super;

  public:
    VideoDeviceMeteor(Parameters const * _params = Parameters::instance());
    virtual ~VideoDeviceMeteor();

    virtual void * grabImage(ACE_Time_Value& _timeStamp) const;

    virtual void connect();
    virtual void disconnect();

  protected:
    virtual	void	setFormat(int);
    virtual	void	setSource(int);
    virtual	void	setPalette(int);
    virtual	void	setSize(int, int);

    int getCurrentErrorCount() const;

    ACE_DEV_Addr devName_;
    ACE_DEV_IO ioBuffer_;
    ACE_DEV_Connector connector_;

    meteor_geomet	meteorGeometry;

    int			videoFd;
    char*		map;
  };
};
#endif

