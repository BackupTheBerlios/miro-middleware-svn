// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoImpl_h
#define VideoImpl_h

#include "video/VideoConsumer.h"
#include "video/Parameters.h"
#include "miro/VideoS.h"
//------------------------ VideoImpl ---------------------------//

namespace Miro {

/**
* VideoImpl
*/

class VideoImpl :  public virtual POA_Miro::Video	//	, public Log
  {
   public:
   	VideoImpl(::Video::Consumer*);
   	virtual ~VideoImpl();

    virtual Miro::ImageHandleIDL connect () throw();
    virtual void release (const Miro::ImageHandleIDL & img) throw();

    virtual void getImage (Miro::ImageHandleIDL & img) throw();
    virtual void getWaitImage (Miro::ImageHandleIDL & img) throw();

    virtual ::Miro::SubImageDataIDL * exportSubImage (CORBA::Long x, CORBA::Long y)
      ACE_THROW_SPEC ((CORBA::SystemException, Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut));

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

