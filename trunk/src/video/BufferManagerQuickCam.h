// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Video_BufferManagerQuickCam_h
#define Video_BufferManagerQuickCam_h

#include "BufferManager.h"

extern "C" {
#include <linux/videodev.h>
#include <pwc-8.11/2.4.21/pwc-ioctl.h>
}

namespace Video
{
  //! Class for managing an array of memory buffers.
  class BufferManagerQuickCam : public BufferManager
  {
    typedef BufferManager Super;

  public:
    //! Initializing constructor.
    BufferManagerQuickCam(Filter const * const _filter,
		      int _fd,
		      int _width,
		      int _height,
		      int _format,
		      unsigned int _buffers,
		      unsigned char * _bufferAddr[]);
    //! Cleaning up.
    ~BufferManagerQuickCam();
    
  protected:
    unsigned int protectedAcquireNextWriteBuffer() throw (Miro::Exception);

    int fd_;
    struct video_mmap * gb_;

    unsigned int leaderBuffer_;
    unsigned int followerBuffer_;
  };
}

#endif // Video_BufferManagerQuickCam_h
