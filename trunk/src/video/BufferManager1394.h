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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "BufferManager.h"

#include <libdc1394/dc1394_control.h>

namespace Video
{
  //! Class for managing an array of memory buffers.
  class BufferManager1394 : public BufferManager
  {
    typedef BufferManager Super;

  public:
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    //! Initializing constructor.
    BufferManager1394(Filter const * const _filter,
		      dc1394_cameracapture *  _pCamera) throw (std::bad_alloc);
#else
    //! Initializing constructor.
    BufferManager1394(Filter const * const _filter,
		      dc1394camera_t *  _pCamera) throw (std::bad_alloc);
#endif

   protected:

    // inherited interface
    virtual unsigned int protectedAcquireNextWriteBuffer() throw (Miro::Exception);
    virtual void protectedSwitchWrite2ReadBuffer(unsigned int _index, unsigned int _n) 
      throw (Miro::EOutOfBounds);
    virtual void protectedReleaseReadBuffer(unsigned int _index) throw (Miro::EOutOfBounds);
    
    // capture management
    void acquireOutputBuffer(unsigned long _index);
    void releaseOutputBuffer();

#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    //! Camera capture data structure.
    dc1394_cameracapture * pCamera_;
#else
    //! Camera capture data structure.
    dc1394camera_t * pCamera_;
#endif
    //! Pointer to the camera parameters of this device
    const Miro::CameraParameters * camParams_;
  };
}

#endif // Video_BufferManager1394_h
