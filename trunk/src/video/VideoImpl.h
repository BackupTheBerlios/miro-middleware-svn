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

#include "miro/VideoS.h"
#include "miro/Synch.h"

#include <set>

// forward declarartions
namespace Video
{
  class BufferManager;
  class VideoInterfaceParameters;
};

//------------------------ VideoImpl ---------------------------//

namespace Miro 
{
  // forward declarartions
  class Server;

  class VideoImpl :  public virtual POA_Miro::Video
  {
  public:
    VideoImpl(Miro::Server& _server, 
	      ::Video::VideoInterfaceParameters const& _params,
	      Miro::ImageFormatIDL const & _format);
    virtual ~VideoImpl();

    unsigned int connections() const;
    ::Video::BufferManager * bufferManager();

    virtual ImageHandleIDL * connect(CORBA::ULong& id) ACE_THROW_SPEC (());
    virtual void disconnect(CORBA::ULong id) 
      ACE_THROW_SPEC ((EOutOfBounds));

    virtual TimeIDL acquireCurrentImage(CORBA::ULong id, CORBA::ULong& buffer) 
      ACE_THROW_SPEC ((EOutOfBounds));
    virtual TimeIDL acquireNextImage(CORBA::ULong id, CORBA::ULong& buffer)
      ACE_THROW_SPEC ((EOutOfBounds, ETimeOut));
    virtual void releaseImage(CORBA::ULong id, CORBA::ULong buffer) 
      ACE_THROW_SPEC((EOutOfBounds));

    virtual SubImageDataIDL * exportWaitSubImage(CORBA::ULong& x, CORBA::ULong& y)  
      ACE_THROW_SPEC ((EOutOfBounds, EDevIO, ETimeOut));

  protected:
    typedef std::set<CORBA::ULong> ClientIdVector;

    Server& server_;
    ::Video::VideoInterfaceParameters const& params_;
    Miro::ImageFormatIDL format_;
    Miro::Video_ptr pVideo;

    unsigned char * pBufferArray_;
    ImageHandleIDL imageHandle_;
    ::Video::BufferManager * pBufferManager_;

    mutable Mutex clientMutex_;
    ClientIdVector clientId_;
    int connections_;

    static CORBA::Long idCounter;
  };

  inline
  ::Video::BufferManager *
  VideoImpl::bufferManager() {
    return pBufferManager_;
  }
};

#endif

