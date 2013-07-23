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

#include "BufferManagerUnicap.h"
#include "video/Parameters.h"
#include "VideoFilter.h"

namespace Video
{
  BufferManagerUnicap::BufferManagerUnicap(Filter const * const _filter,
					   unsigned int _buffers, 
					   unsigned int _bufferSize,
					   unicap_handle_t _handle,
					   unicap_format_t _format)
    throw (std::bad_alloc) :
    Super(_filter, _buffers, _bufferSize, NULL),
    handle_(_handle),
    format_(_format)
  {
    // get device parameters
    const Video::DeviceParameters * devParams =
      dynamic_cast<DeviceParameters const *>(_filter->parameters());
    camParams_ = &devParams->camera;
    MIRO_LOG_OSTR(LL_NOTICE, "\n  camera parameters:\n" << *camParams_);

    // init callback structure
    callback_.index = 0;
    callback_.mutex = new Miro::Mutex();
    callback_.condition = new Miro::Condition(*callback_.mutex);
    callback_.buffer = new unsigned char * [buffers()];
    for (unsigned int i=0; i<buffers(); ++i)
      callback_.buffer[i] = bufferAddr(i);

    // register callback
    unicap_register_callback(handle_, UNICAP_EVENT_NEW_FRAME, (unicap_callback_t) newFrameCallback, (void*)&callback_);

    // Start the capture process on the device
    if( !SUCCESS( unicap_start_capture( handle_ ) ) )
      throw Miro::Exception("DeviceUnicap: Failed to start capture on unicap device");
  }


  unsigned int 
  BufferManagerUnicap::protectedAcquireNextWriteBuffer() throw (Miro::Exception)
  {
    callback_.index = Super::protectedAcquireNextWriteBuffer();

    bufferStatus_[callback_.index].time = ACE_OS::gettimeofday() - camParams_->latency;

    Miro::Guard guard(*callback_.mutex);
    callback_.condition->wait();
    return callback_.index;
  }


  void
  BufferManagerUnicap::newFrameCallback(unicap_event_t, unicap_handle_t,
					unicap_data_buffer_t * buffer, void *usr_data)
  {
    CallbackData *callback = (CallbackData*)usr_data;

    Miro::Guard guard(*callback->mutex);
    memcpy(callback->buffer[callback->index], buffer->data, buffer->buffer_size);
    callback->condition->broadcast();
  }
}
