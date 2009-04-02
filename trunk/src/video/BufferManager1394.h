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
#ifndef Video_BufferManager1394_h
#define Video_BufferManager1394_h

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "BufferManager.h"

#include <libraw1394/raw1394.h>
#include <dc1394/control.h>

namespace Video
{
  //! Class for managing an array of memory buffers.
  class BufferManager1394 : public BufferManager
  {
    typedef BufferManager Super;

  public:
    //! Initializing constructor.
    BufferManager1394(Filter const * const _filter,
		      dc1394camera_t *  _pCamera) throw (std::bad_alloc);

   protected:

    // inherited interface
    virtual unsigned int protectedAcquireNextWriteBuffer() throw (Miro::Exception);
    virtual void protectedSwitchWrite2ReadBuffer(unsigned int _index, unsigned int _n) 
      throw (Miro::EOutOfBounds);
    virtual void protectedReleaseReadBuffer(unsigned int _index) throw (Miro::EOutOfBounds);
    
    // capture management
    void acquireOutputBuffer(unsigned long _index);
    void releaseOutputBuffer();

    dc1394video_frame_t *frame;

    //! Camera capture data structure.
    dc1394camera_t * pCamera_;

    //! Pointer to the camera parameters of this device
    const Miro::CameraParameters * camParams_;
  };
}

#endif // Video_BufferManager1394_h
