// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef Video_BufferManagerQuickCam_h
#define Video_BufferManagerQuickCam_h

#include "BufferManager.h"

extern "C" {
#include <linux/videodev.h>
#include "pwc-ioctl.h"
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

    //! Pointer to the camera parameters of this device
    const Miro::CameraParameters * camParams_;
  };
}

#endif // Video_BufferManagerQuickCam_h
