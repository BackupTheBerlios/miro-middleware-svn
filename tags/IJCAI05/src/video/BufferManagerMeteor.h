// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
