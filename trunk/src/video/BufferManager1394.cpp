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

#include "BufferManager1394.h"

#include <ace/OS.h>

namespace 
{
  unsigned char * b[1] = { NULL };
}

namespace Video
{
  BufferManager1394::BufferManager1394(Filter const * const _filter,
				       dc1394_cameracapture *  _pCamera) 
    throw (std::bad_alloc) :
    Super(_filter, 1, b),
    pCamera_(_pCamera)
  {
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
    dc1394_dma_single_capture(pCamera_);

    bufferStatus_[_index].time = ACE_OS::gettimeofday();
    bufferStatus_[_index].buffer = 
      reinterpret_cast<unsigned char *>(pCamera_->capture_buffer);
  }

  //---------------------------------------------------------------
  void
  BufferManager1394::releaseOutputBuffer()
  {	
    dc1394_dma_done_with_buffer(pCamera_);
  }
}
