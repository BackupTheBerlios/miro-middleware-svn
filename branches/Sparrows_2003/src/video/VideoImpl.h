// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoImpl_h
#define VideoImpl_h

#include "VideoConsumer.h"
#include "Parameters.h"

#include "miro/VideoS.h"

//------------------------ VideoImpl ---------------------------//

namespace Miro 
{
  class VideoImpl :  public virtual POA_Miro::Video
  {
  public:
    VideoImpl(::Video::Consumer*);
    virtual ~VideoImpl();

    virtual Miro::ImageHandleIDL connect() throw();
    virtual void release(const Miro::ImageHandleIDL & img) ACE_THROW_SPEC (());

    virtual Miro::TimeIDL getImage(Miro::ImageHandleIDL & img) ACE_THROW_SPEC (());
    virtual Miro::TimeIDL getWaitImage(Miro::ImageHandleIDL & img) ACE_THROW_SPEC (());

    virtual SubImageDataIDL * exportWaitSubImage(CORBA::Long x, CORBA::Long y)  ACE_THROW_SPEC ((CORBA::SystemException, Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut));

  protected:
    void checkImageHandle(const Miro::ImageHandleIDL & img);

    ::Video::Consumer*  pConsumer;
    const ::Video::Parameters&  parameters;
  	
    int	iMaxConnections;
    void** pShmDataArray;
    Miro::ImageHandleIDL*	pHandleArray;
  };
};

#endif

