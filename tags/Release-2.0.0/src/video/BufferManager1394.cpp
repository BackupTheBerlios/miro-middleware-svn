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
#include "BufferManager1394.h"
#include "video/Parameters.h"
#include "VideoFilter.h"

#include <ace/OS.h>

namespace 
{
  unsigned char * b[1] = { NULL };
}

namespace Video
{
#if MIRO_HAS_LIBDC1394_VERSION == 1 || MIRO_HAS_LIBDC1394_VERSION == 2
  BufferManager1394::BufferManager1394(Filter const * const _filter,
				       dc1394_cameracapture *  _pCamera) 
#else
  BufferManager1394::BufferManager1394(Filter const * const _filter,
				       dc1394camera_t * _pCamera) 
#endif
    throw (std::bad_alloc) :
    Super(_filter, 1, b),
    pCamera_(_pCamera)
  {
    const Video::DeviceParameters * devParams =
      dynamic_cast<DeviceParameters const *>(_filter->parameters());
    camParams_ = &devParams->camera;
    MIRO_LOG_OSTR(LL_NOTICE, "\n  camera parameters:\n" << *camParams_);
  }

  unsigned int 
  BufferManager1394::protectedAcquireNextWriteBuffer() throw (Miro::Exception)
  {
    unsigned int index = Super::protectedAcquireNextWriteBuffer();
    acquireOutputBuffer(index);
    return index;
  }

  void 
  BufferManager1394::protectedSwitchWrite2ReadBuffer(unsigned int _index, unsigned int _n) 
    throw (Miro::EOutOfBounds)
  {
    Super::protectedSwitchWrite2ReadBuffer(_index, _n);
    if (bufferStatus_[_index].state == BufferEntry::FREE)
      releaseOutputBuffer();
  }

  void
  BufferManager1394::protectedReleaseReadBuffer(unsigned int _index) 
    throw (Miro::EOutOfBounds)
  {
    Super::protectedReleaseReadBuffer(_index);
    if (bufferStatus_[_index].state == BufferEntry::FREE)
      releaseOutputBuffer();
  }


  //---------------------------------------------------------------
  void
  BufferManager1394::acquireOutputBuffer(unsigned long _index)
  {
#if MIRO_HAS_LIBDC1394_VERSION == 1 || MIRO_HAS_LIBDC1394_VERSION == 2
    dc1394_dma_single_capture(pCamera_);
    bufferStatus_[_index].time = ACE_OS::gettimeofday() - camParams_->latency;
    bufferStatus_[_index].buffer = reinterpret_cast<unsigned char *>(pCamera_->capture_buffer);
#else
    dc1394_capture_dma(&pCamera_, 1, DC1394_VIDEO1394_WAIT);
    bufferStatus_[_index].time = ACE_OS::gettimeofday() - camParams_->latency;
    bufferStatus_[_index].buffer = reinterpret_cast<unsigned char *>(pCamera_->capture.capture_buffer);
#endif
  }

  //---------------------------------------------------------------
  void
  BufferManager1394::releaseOutputBuffer()
  {	
#if MIRO_HAS_LIBDC1394_VERSION == 1 || MIRO_HAS_LIBDC1394_VERSION == 2
    dc1394_dma_done_with_buffer(pCamera_);
#else
    dc1394_capture_dma_done_with_buffer(pCamera_);
#endif
  }
}