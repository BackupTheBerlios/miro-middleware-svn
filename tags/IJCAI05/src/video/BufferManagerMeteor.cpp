// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BufferManagerMeteor.h"
#include "Parameters.h"
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
