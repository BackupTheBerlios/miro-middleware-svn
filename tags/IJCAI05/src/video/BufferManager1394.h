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
#ifndef Video_BufferManager1394_h
#define Video_BufferManager1394_h

#include "BufferManager.h"

#include <libdc1394/dc1394_control.h>

namespace Video
{
  //! Class for managing an array of memory buffers.
  class BufferManager1394 : public BufferManager
  {
    typedef BufferManager Super;

  public:
    //! Initializing constructor.
    BufferManager1394(Filter const * const _filter,
		      dc1394_cameracapture *  _pCamera) throw (std::bad_alloc);

   protected:

    // inherited interface
    virtual unsigned int protectedAcquireNextWriteBuffer() throw (Miro::Exception);
    virtual void protectedSwitchWrite2ReadBuffer(unsigned int _index, unsigned int _n) 
      throw (Miro::EOutOfBounds);
    virtual void protectedReleaseReadBuffer(unsigned int _index) throw (Miro::EOutOfBounds);
    
    // capture management
    void acquireOutputBuffer(unsigned long _index);
    void releaseOutputBuffer();

    //! Camera capture data structure.
    dc1394_cameracapture *  pCamera_;
    //! Pointer to the camera parameters of this device
    const Miro::CameraParameters * camParams_;
  };
}

#endif // Video_BufferManager1394_h
