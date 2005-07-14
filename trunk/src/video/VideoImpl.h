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

#include "Parameters.h"

#include "idl/VideoS.h"
#include "miro/Synch.h"

#include <set>
#include <string>

// forward declarartions
namespace Video
{
  class Filter;
  class BufferManager;
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
	      ::Video::Filter * _filter);
    virtual ~VideoImpl();

    Video_ptr ior() const;
    bool checkClientId(CORBA::ULong _id, bool _nothrow = false) const;
    unsigned int connections() const;
    const std::string&  name() const;
    ::Video::BufferManager * bufferManager();

    virtual void getParameters(CORBA::String_out _name,
			       CORBA::String_out _type,
			       CORBA::String_out _document) throw();
    virtual void setParameters(char const * _document) throw();

    virtual ImageHandleIDL * connect(CORBA::ULong& id) ACE_THROW_SPEC (());
    virtual void disconnect(CORBA::ULong id) 
      ACE_THROW_SPEC ((EOutOfBounds));

    virtual TimeIDL acquireCurrentImage(CORBA::ULong id, CORBA::ULong& buffer) 
      ACE_THROW_SPEC ((EOutOfBounds));
    virtual TimeIDL acquireNextImage(CORBA::ULong id, CORBA::ULong& buffer)
      ACE_THROW_SPEC ((EOutOfBounds, ETimeOut));
    virtual void releaseImage(CORBA::ULong id, CORBA::ULong buffer) 
      ACE_THROW_SPEC((EOutOfBounds));

    virtual SubImageDataIDL * exportSubImage(CORBA::ULong& x, CORBA::ULong& y)  
      ACE_THROW_SPEC ((EOutOfBounds, EDevIO));
    virtual SubImageDataIDL * exportWaitSubImage(CORBA::ULong& x, CORBA::ULong& y)  
      ACE_THROW_SPEC ((EOutOfBounds, EDevIO, ETimeOut));

  protected:
    SubImageDataIDL * localExportSubImage(CORBA::ULong& x, CORBA::ULong& y, bool _wait)  
      ACE_THROW_SPEC ((EOutOfBounds, EDevIO, ETimeOut));

    typedef std::set<CORBA::ULong> ClientIdVector;

    Server& server_;
    ::Video::VideoInterfaceParameters const& params_;
    ::Video::Filter * filter_;
    Miro::Video_ptr pVideo;

    unsigned char * pBufferArray_;
    ImageHandleIDL imageHandle_;
    ::Video::BufferManager * pBufferManager_;

    mutable Mutex clientMutex_;
    ClientIdVector clientId_;

    static CORBA::Long idCounter;
  };

  inline
  Video_ptr
  VideoImpl::ior() const {
    return pVideo;
  }

  inline
  const std::string&
  VideoImpl::name() const {
    return params_.name;
  }

  inline
  ::Video::BufferManager *
  VideoImpl::bufferManager() {
    return pBufferManager_;
  }
}

#endif

