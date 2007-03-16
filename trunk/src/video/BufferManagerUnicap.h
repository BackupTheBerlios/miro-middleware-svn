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

#ifndef BUFFERMANAGERUNICAP_H
#define BUFFERMANAGERUNICAP_H

#include "BufferManager.h"

#include "miro/Synch.h"

#include <unicap.h>
#include <unicap_status.h>

namespace Video
{
  class BufferManagerUnicap : public BufferManager
  {
    typedef BufferManager Super;

  public:
    BufferManagerUnicap(Filter const * const _filter,
			unsigned int _buffers, 
			unsigned int _bufferSize,
			unicap_handle_t _handle,
			unicap_format_t _format)
      throw (std::bad_alloc);

  protected:
    static void newFrameCallback(unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t * buffer, void *usr_data);
    virtual unsigned int protectedAcquireNextWriteBuffer() throw (Miro::Exception);

    unicap_handle_t handle_;
    unicap_format_t format_;

    //! Pointer to the camera parameters of this device
    const Miro::CameraParameters * camParams_;

    typedef struct CallbackData {
      // buffer index
      long index;
      // pointer to bufferStatus.buffer
      unsigned char ** buffer;
      // sync callback and write buffer acquisition
      Miro::Mutex * mutex;
      Miro::Condition * condition;
    };

    CallbackData callback_;
  };
}

#endif
