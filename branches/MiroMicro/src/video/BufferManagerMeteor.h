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
#ifndef Video_BufferManagerMeteor_h
#define Video_BufferManagerMeteor_h

#include "BufferManager.h"

namespace Video
{
  //! Class for managing an array of memory buffers.
  class BufferManagerMeteor : public BufferManager
  {
    typedef BufferManager Super;

  public:
    //! Initializing constructor.
    BufferManagerMeteor(Filter const * const _filter, 
			int _fd, unsigned int _size, unsigned char * _buffer)
      throw (std::bad_alloc);

  protected:

    // inherited interface
    virtual unsigned int protectedAcquireNextWriteBuffer() throw (Miro::Exception);

    // capture management
    void acquireOutputBuffer(unsigned long _index);
    int getCurrentErrorCount() const;

    int fd_;
    //! Pointer to the camera parameters of this device
    const Miro::CameraParameters * camParams_;
  };
}

#endif // Video_BufferManagerMeteor_h
