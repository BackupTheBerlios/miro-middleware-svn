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
#include "BufferManagerMeteor.h"
#include "video/Parameters.h"
#include "VideoFilter.h"

// extern "C" {
#include "ioctl_meteor.h"
// }

namespace Video
{
  BufferManagerMeteor::BufferManagerMeteor(Filter const * const _filter,
					   int _fd,
					   unsigned int _bufferSize,
					   unsigned char * _memory) 
    throw (std::bad_alloc) :
    Super(_filter, 1, _bufferSize, _memory),
    fd_(_fd)
  {
    const Video::DeviceParameters * devParams =
      dynamic_cast<DeviceParameters const *>(_filter->parameters());
    camParams_ = &devParams->camera;
    MIRO_LOG_OSTR(LL_NOTICE, "\n  camera parameters:\n" << *camParams_);
  }

  unsigned int 
  BufferManagerMeteor::protectedAcquireNextWriteBuffer() throw (Miro::Exception)
  {
    unsigned int index = Super::protectedAcquireNextWriteBuffer();
    acquireOutputBuffer(index);
    return index;
  }

  //---------------------------------------------------------------
  void
  BufferManagerMeteor::acquireOutputBuffer(unsigned long _index)
  {
    int		iNTries = 0;
    int		iNCaptureRetries = 16;
    bool	done = false;

    //	capture one frame
    int	command = METEOR_CAP_SINGLE;
    int	errorCount = getCurrentErrorCount();

    if (::ioctl(fd_, METEORCAPTUR, &command) < 0)
      throw Miro::Exception("METEORCAPTUR");

    while (!done && 
	   iNCaptureRetries && 
	   iNTries < iNCaptureRetries) {
      int currentErrorCount = getCurrentErrorCount();
      if (currentErrorCount == errorCount) {
	done = true;
      }
      else {
	iNTries++;
	errorCount = currentErrorCount;
	command = METEOR_CAP_SINGLE;
	if (::ioctl(fd_, METEORCAPTUR, &command) < 0)
	  throw Miro::Exception("METEORCAPTUR");
      }
    }
    if (!done)
      throw Miro::Exception("DeviceMeteor::grabImage");

    bufferStatus_[_index].time = ACE_OS::gettimeofday() - camParams_->latency;

  }

  //---------------------------------------------------------------
  int 
  BufferManagerMeteor::getCurrentErrorCount() const
  {
    struct meteor_counts cnts;
    if (::ioctl(fd_, METEORGCOUNT, &cnts) < 0)
      throw Miro::Exception("METEORGCOUNT");
    return cnts.fifo_errors + cnts.dma_errors;
  }
}
