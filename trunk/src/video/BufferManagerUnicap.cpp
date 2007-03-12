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
    const Video::DeviceParameters * devParams =
      dynamic_cast<DeviceParameters const *>(_filter->parameters());
    camParams_ = &devParams->camera;
    MIRO_LOG_OSTR(LL_NOTICE, "\n  camera parameters:\n" << *camParams_);
  }


  unsigned int 
  BufferManagerUnicap::protectedAcquireNextWriteBuffer() throw (Miro::Exception)
  {
    unsigned int index = Super::protectedAcquireNextWriteBuffer();
    acquireOutputBuffer(index);
    return index;
  }


  void
  BufferManagerUnicap::acquireOutputBuffer(unsigned long _index)
  {
    unicap_data_buffer_t buffer;
    buffer.data = bufferStatus_[_index].buffer;
    buffer.buffer_size = format_.buffer_size;

    // Queue the buffer. The buffer now gets filled with image data by the capture device
    if( !SUCCESS( unicap_queue_buffer( handle_, &buffer ) ) )
      throw Miro::Exception("BufferManagerUnicap: Failed to queue a buffer on device");

    unicap_data_buffer_t *returned_buffer;
	
    // Wait until the image buffer is ready
    if( !SUCCESS( unicap_wait_buffer( handle_, &returned_buffer ) ) )
      throw Miro::Exception("BufferManagerUnicap: Failed to wait for buffer on device");

    bufferStatus_[_index].time = ACE_OS::gettimeofday() - camParams_->latency;
  }
}
