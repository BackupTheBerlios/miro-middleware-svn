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
    BufferManagerMeteor(int _fd, unsigned int _size, unsigned char * _buffer)
      throw (std::bad_alloc);

  protected:

    // inherited interface
    virtual unsigned int protectedAcquireNextWriteBuffer() throw (Miro::Exception);
    virtual void protectedSwitchWrite2ReadBuffer(unsigned int _index, unsigned int _n) 
      throw (Miro::EOutOfBounds);
    virtual void protectedReleaseReadBuffer(unsigned int _index) throw (Miro::EOutOfBounds);
    
    // capture management
    void acquireOutputBuffer(unsigned long _index);
    int getCurrentErrorCount() const;

    int fd_;
  };
}

#endif // Video_BufferManagerMeteor_h
