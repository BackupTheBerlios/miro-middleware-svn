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
#include "VideoAVDevice.h"

#include <ace/DEV_Connector.h>

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  class DeviceMeteor  : public AVDevice
  {
    typedef AVDevice Super;

  public:
    DeviceMeteor(const Miro::ImageFormatIDL& _inputFormatParameters);
    virtual ~DeviceMeteor();

  protected:
    virtual BufferManager * bufferManagerInstance() const;
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void fini();

    void setFormat();
    void setSource();
    void setPalette();
    void setSize();

    // protected data

    AVDeviceParameters const * params_;

    ACE_DEV_Addr devName_;
    ACE_DEV_IO ioBuffer_;
    ACE_DEV_Connector connector_;

    meteor_geomet	meteorGeometry;

    unsigned char * buffer_;
  };
};
#endif

