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

#include "BufferManagerQuickCam.h"
#include <ace/OS.h>

namespace Video
{
  BufferManagerQuickCam::BufferManagerQuickCam(Filter const * const _filter,
				       int _fd,
				       int _width,
				       int _height,
				       int _format,
				       unsigned int _buffers, 
				       unsigned char * _bufferAddr[]) :
    Super(_filter, _buffers, _bufferAddr),
    fd_(_fd),
    gb_(new struct video_mmap[_buffers]),
    leaderBuffer_(0),
    followerBuffer_(0)
  {
    // initialize
    for (unsigned long i = 0; i < _buffers; ++i) {
      gb_[i].width  = _width;
      gb_[i].height = _height;
      gb_[i].format = _format;
      gb_[i].frame = i;
    }

    leaderBuffer_ = Super::protectedAcquireNextWriteBuffer();
    int err = ::ioctl(fd_, VIDIOCMCAPTURE, &(gb_[leaderBuffer_]));
    followerBuffer_ = leaderBuffer_;
 /*   if (err != -1) {
      if (buffers_ > 2) {
	followerBuffer_ = Super::protectedAcquireNextWriteBuffer();
	err = ::ioctl(fd_, VIDIOCMCAPTURE, &(gb_[followerBuffer_]));
      }
      else {
	followerBuffer_ = leaderBuffer_;
      }
    }*/
    if (err == -1)
      throw Miro::CException(errno, "BufferManagerQuickCam::BufferManagerQuickCam() - VIDIOCMCAPTURE");
  }

  /** Deleting memory_ if it is its own_. */
  BufferManagerQuickCam::~BufferManagerQuickCam()
  {
    delete gb_;
  }

  unsigned int
  BufferManagerQuickCam::protectedAcquireNextWriteBuffer() throw (Miro::Exception)
  {
    // if we have only two frames, we can't grab forward
   /* if (buffers_ <= 2) {
      leaderBuffer_ = followerBuffer_;
    }*/

    // synch current image
    int err = ::ioctl(fd_, VIDIOCSYNC, &leaderBuffer_);
    if (err == -1) {
      throw Miro::CException(errno, "BufferManagerQuickCam::acquireBuffer() - VIDIOCSYNC");
    }

    // set time stamp
    bufferStatus_[leaderBuffer_].time = ACE_OS::gettimeofday() - camParams_->latency;

    // update the leader/follower buffer index
    int index = leaderBuffer_;

    followerBuffer_ = Super::protectedAcquireNextWriteBuffer();

    // grab next image
    err = ::ioctl(fd_, VIDIOCMCAPTURE, &(gb_[followerBuffer_]));
    if (err == -1) {
      throw Miro::CException(errno, "BufferManagerQuickCam::acquireBuffer() - VIDIOCMCAPTURE");
    }

    leaderBuffer_ = followerBuffer_;

    return index;
  }
}
