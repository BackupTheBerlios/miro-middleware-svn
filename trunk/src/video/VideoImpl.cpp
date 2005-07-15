// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#include "miro/Server.h"
#include "miro/TimeHelper.h"
#include "miro/VideoHelper.h"
#include "miro/Exception.h"
#include "miro/Log.h"
#include "miro/ConfigDocument.h"

#include "VideoImpl.h"
#include "VideoFilter.h"
#include "VideoConsumer.h"
#include "BufferManager.h"
#include "Parameters.h"

#include <qdom.h>

#include <sys/ipc.h>
#include <sys/shm.h>

namespace Miro 
{
  using namespace Video;

  CORBA::Long VideoImpl::idCounter = 0;

  VideoImpl::VideoImpl(Server& _server, 
		       ::Video::VideoInterfaceParameters const & _params,
		       ::Video::Filter * _filter) :
    server_(_server),
    params_(_params),
    filter_(_filter),
    pBufferManager_(NULL)
  {
    MIRO_LOG_CTOR("Miro::VideoImpl");

    imageHandle_.format = filter_->outputFormat();
    imageHandle_.key = shmget(0, 
			      getImageSize(filter_->outputFormat()) * filter_->outputBuffers(),
			      IPC_CREAT|0x1ff);
    if (imageHandle_.key == -1) {
      throw Miro::CException(errno, "Failed creating shared memory segment!");
    }
    pBufferArray_ = (unsigned char*)shmat(imageHandle_.key, 0, 0);
    if (pBufferArray_ == (void*)-1) {
	  throw Miro::CException(errno, "Failed attaching shared memory segment!");
    }

    imageHandle_.offset.length(filter_->outputBuffers());
    for (unsigned int i = 0; i < filter_->outputBuffers(); ++i) {
      imageHandle_.offset[i] = i * getImageSize(filter_->outputFormat());
    }

    pBufferManager_ = new BufferManager(_filter,
					filter_->outputBuffers(),
					getImageSize(filter_->outputFormat()),
					pBufferArray_);

    // register the video interface at the POA
    pVideo = this->_this();
    server_.addToNameService(pVideo, params_.name.c_str());
  }
  
  VideoImpl::~VideoImpl()
  {
    MIRO_LOG_DTOR("Miro::VideoImpl");

    // Deactivate the interfaces.
    // we have to do this manually for none owned orbs,
    // as the class goes out of scope before
    // the orb is shut down
    PortableServer::ObjectId_var oid;
    oid =  server_.poa->reference_to_id(pVideo);
    server_.poa->deactivate_object(oid.in());

    // clean up shared memory
    delete pBufferManager_;
    shmdt(pBufferArray_);
    shmctl(imageHandle_.key, IPC_RMID, NULL);
  }

  void 
  VideoImpl::getParameters(CORBA::String_out _name,
			   CORBA::String_out _type,
			   CORBA::String_out _document) throw()
  {
    _name = CORBA::string_dup(filter_->name().c_str());
    _type = CORBA::string_dup("dunno");
    QDomDocument document("MiroConfigDocument");
    QDomElement root = document.createElement("config");
    QDomNode config = document.appendChild( root );
    QDomElement e = document.createElement("section");
    e.setAttribute("name", "Video");
    QDomNode section = config.appendChild(e);

    QDomElement instance = *(filter_->parameters()) >>= section;
    instance.setTagName("instance");
    instance.setAttribute("type", filter_->parameters()->fullTypeName());
    instance.setAttribute("name", filter_->name());

    _document = CORBA::string_dup(document.toCString());
  }

  void 
  VideoImpl::setParameters(char const * _document) throw(EOutOfBounds)
  {
    QDomDocument * doc = new QDomDocument("MiroConfigDocument");

    QCString p(_document);
    if (!doc->setContent(p)) {
	throw EOutOfBounds();
    }

    Miro::ConfigDocument document(doc);
    document.setSection("Video");

    // create an instance of the the filters parameters
    FilterParameters * params = filter_->getParametersInstance();

    // initialize the parameter instance from the config file
    // _config.getInstance(this->name(), *params);
    document.getInstance(filter_->name(), *params);

    // debug output
    MIRO_LOG_OSTR(LL_NOTICE, name() << std::endl << *params);

    // initialize the filter instance with its parameters
    filter_->init(server_, params); 
  }

  unsigned int
  VideoImpl::connections() const
  {
    Guard guard(clientMutex_);
    return clientId_.size();
  }
  
  Miro::ImageHandleIDL *
  VideoImpl::connect(CORBA::ULong& _id) ACE_THROW_SPEC(())
  {
    {
      Guard guard(clientMutex_);
      _id = ++idCounter;
      clientId_.insert(_id);
    }

    filter_->connect();

    return new Miro::ImageHandleIDL(imageHandle_);
  }

  void
  VideoImpl::disconnect(CORBA::ULong _id) 
    ACE_THROW_SPEC((EOutOfBounds))
  {
    Miro::Guard guard(clientMutex_);
    if (clientId_.erase(_id) == 0)
      throw Miro::EOutOfBounds("Unregistered client id.");

    filter_->disconnect();
  }

  TimeIDL
  VideoImpl::acquireCurrentImage(CORBA::ULong _id, CORBA::ULong& _buffer) 
    ACE_THROW_SPEC((EOutOfBounds))
  {
    TimeIDL stamp;
    checkClientId(_id);

    _buffer = pBufferManager_->acquireCurrentReadBuffer();
    timeA2C(pBufferManager_->bufferTimeStamp(_buffer), stamp);

    return stamp;
  }

  TimeIDL
  VideoImpl::acquireNextImage(CORBA::ULong _id, CORBA::ULong& _buffer) 
    ACE_THROW_SPEC((EOutOfBounds, ETimeOut))
  {
    TimeIDL stamp;
    checkClientId(_id);

    _buffer = pBufferManager_->acquireNextReadBuffer();
    timeA2C(pBufferManager_->bufferTimeStamp(_buffer), stamp);

    return stamp;
  }

  void
  VideoImpl::releaseImage(CORBA::ULong _id, CORBA::ULong _buffer)
    ACE_THROW_SPEC((EOutOfBounds))
  {
    checkClientId(_id);
    pBufferManager_->releaseReadBuffer(_buffer);
  }

  SubImageDataIDL *
  VideoImpl::exportSubImage(CORBA::ULong& x, CORBA::ULong& y)
    ACE_THROW_SPEC((EOutOfBounds, EDevIO))
  {
    return localExportSubImage(x, y, false);
  }

  SubImageDataIDL *
  VideoImpl::exportWaitSubImage(CORBA::ULong& x, CORBA::ULong& y)
    ACE_THROW_SPEC((EOutOfBounds, EDevIO, ETimeOut))
  {
    return localExportSubImage(x, y, true);
  }

  SubImageDataIDL *
  VideoImpl::localExportSubImage(CORBA::ULong& x, CORBA::ULong& y, bool _wait)
    ACE_THROW_SPEC((EOutOfBounds, EDevIO, ETimeOut))
  {
    int             bufferSize = ::Miro::getImageSize(filter_->outputFormat());
    unsigned char   * dst   = new unsigned char[bufferSize];
    SubImageDataIDL * subImage = new SubImageDataIDL(bufferSize, bufferSize, dst, 1);

    unsigned int bufferId = (_wait)?
      pBufferManager_->acquireNextReadBuffer() :
      pBufferManager_->acquireCurrentReadBuffer();

    unsigned char const * src = pBufferManager_->bufferAddr(bufferId);

    unsigned int srcWidth  = imageHandle_.format.width;
    unsigned int srcHeight = imageHandle_.format.height;

    // do not expand
    x = std::min(srcWidth, x);
    y = std::min(srcHeight, y);

    // src pixels per requested pixel
    double intervalWidth  = (double)srcWidth  / (double)x;
    double intervalHeight = (double)srcHeight / (double)y;

    const int paletteSize = getPixelSize(imageHandle_.format.palette);
    unsigned long * tileValueSum = new unsigned long[paletteSize];

    for (unsigned int req_h = 0; req_h < y; ++req_h) {
      double low_h = (double)(req_h) * intervalHeight;
      double up_h  = (double)(req_h + 1) * intervalHeight;
		
      for (unsigned int req_w = 0; req_w < x; ++req_w) {
	double low_w = (double)(req_w) * intervalWidth;
	double up_w  = (double)(req_w + 1) * intervalWidth;

	long num_in_tile = 0;
	for (int palette = 0; palette < paletteSize; ++palette) 
	  tileValueSum[palette] = 0;
			
	for (int src_h = (int)low_h; src_h < (int)up_h; ++src_h) {
	  for (int src_w = (int)low_w; src_w < (int)up_w; ++src_w) {
	    for (int palette = 0; palette < paletteSize; ++palette) {
	      tileValueSum[palette] += 
		src[paletteSize * (src_h * srcWidth + src_w) + palette];
	    }
	    ++num_in_tile;
	  }
	}

	for (int palette = 0; palette < paletteSize; ++palette) {
	  tileValueSum[palette] /= num_in_tile;
	  dst[paletteSize * (req_h * x + req_w) + palette] = 
	    (unsigned char)tileValueSum[palette];
	}
      }
    }

    delete tileValueSum;

    pBufferManager_->releaseReadBuffer(bufferId);
    return subImage;
  }

  bool
  VideoImpl::checkClientId(CORBA::ULong _id, bool _nothrow) const
  {
    Guard guard(clientMutex_);
    bool found = clientId_.find(_id) != clientId_.end();
    if (!found && !_nothrow)
      throw Miro::EOutOfBounds("Unregistered client id.");
    return found;
  }
}
