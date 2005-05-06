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
#ifndef Miro_VideoBrokerImpl_h
#define Miro_VideoBrokerImpl_h

#include "idl/VideoS.h"

namespace Video
{
  class Device;
}

namespace Miro
{
  //! Implementation of the Miro::VidoBroker interface.
  class  VideoBrokerImpl : public virtual POA_Miro::VideoBroker
  {
  public:
    //! Initializing constructor.
    /** @param filter The root of the filter tree. */
    VideoBrokerImpl(::Video::Device * _device);
    //! Virtual destructor. 
    virtual ~VideoBrokerImpl();
  
    //! See VideoBroker interface.
    virtual TimeIDL acquireNextImageSet (const ConnectionSetIDL & connections,
					 BufferSetIDL_out buffers)
      ACE_THROW_SPEC ((EOutOfBounds, EDevIO, ETimeOut));

    //! See VideoBroker interface.
    virtual void releaseImageSet (const ConnectionSetIDL & connections,
				  const BufferSetIDL & buffers)
      ACE_THROW_SPEC ((EOutOfBounds));

    //! See VideoBroker interface.
    virtual FilterTreeIDL * filterTreeStats() ACE_THROW_SPEC (());

  protected:
    ::Video::Device * device_;
  };
}

#endif /* Miro_VideoBrokerImpl_h  */
