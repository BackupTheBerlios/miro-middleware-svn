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

#include "BufferManagerBTTV.h"

namespace Video
{
  BufferManagerBTTV::BufferManagerBTTV(Filter const * const _filter,
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
    if (err != -1) {
      followerBuffer_ = Super::protectedAcquireNextWriteBuffer();
      err = ::ioctl(fd_, VIDIOCMCAPTURE, &(gb_[followerBuffer_]));
    }
    if (err == -1)
      throw Miro::CException(errno, "BufferManagerBTTV::BufferManagerBTTV() - VIDIOCMCAPTURE");
  }

  /** Deleting memory_ if it is its own_. */
  BufferManagerBTTV::~BufferManagerBTTV() 
  {
    delete gb_;
  }

  unsigned int
  BufferManagerBTTV::protectedAcquireNextWriteBuffer() throw (Miro::Exception)
  {
    // synch current image
    int err = ::ioctl(fd_, VIDIOCSYNC, &leaderBuffer_);
    if (err == -1) {
      throw Miro::CException(errno, "BufferManagerBTTV::acquireBuffer() - VIDIOCSYNC");
    }

    // set time stamp
    bufferStatus_[leaderBuffer_].time = ACE_OS::gettimeofday();

    // set buffer
    // --> FIXME per init
    // bufferStatus_[_index].buffer = map_ + gb_buffers.offsets[currentBuffer_];

    // update the leader/follower buffer index
    int index = leaderBuffer_;
    leaderBuffer_ = followerBuffer_;
    followerBuffer_ = Super::protectedAcquireNextWriteBuffer();

    // grab next image
    err = ::ioctl(fd_, VIDIOCMCAPTURE, &(gb_[followerBuffer_]));
    if (err == -1) {
      throw Miro::CException(errno, "BufferManagerBTTV::acquireBuffer() - VIDIOCMCAPTURE");
    }

    return index;
  }
}
