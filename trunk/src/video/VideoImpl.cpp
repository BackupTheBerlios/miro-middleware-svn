// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#include "VideoImpl.h"
#include "VideoDevice.h"
#include "VideoConsumer.h"
#include "BufferManager.h"
#include "Parameters.h"

#include "miro/Server.h"
#include "miro/TimeHelper.h"
#include "miro/VideoHelper.h"
#include "miro/Exception.h"

#include <sys/ipc.h>
#include <sys/shm.h>

using namespace Video;

namespace Miro 
{

  CORBA::Long VideoImpl::idCounter = 0;

  VideoImpl::VideoImpl(Server& _server, 
		       ::Video::VideoInterfaceParameters const & _params,
		       Miro::ImageFormatIDL const & _format) :
    server_(_server),
    params_(_params),
    format_(_format),
    pBufferManager_(NULL)
  {
    cout << "Constructing VideoImpl." << endl;

    imageHandle_.format = _format;
    imageHandle_.key = shmget(0, 
			      getImageSize(format_) * params_.buffers,
			      IPC_CREAT|0x1ff);
    if (imageHandle_.key == -1) {
      throw Miro::CException(errno, "Failed creating shared memory segment!");
    }
    pBufferArray_ = (unsigned char*)shmat(imageHandle_.key, 0, 0);
    if (pBufferArray_ == (void*)-1) {
	  throw Miro::CException(errno, "Failed attaching shared memory segment!");
    }

    imageHandle_.offset.length(params_.buffers);
    for (unsigned int i = 0; i < params_.buffers; ++i) {
      imageHandle_.offset[i] = i * getImageSize(format_);
    }

    pBufferManager_ = new BufferManager(params_.buffers,
					getImageSize(format_),
					pBufferArray_);

    // register the video interface at the POA
    pVideo = this->_this();
    server_.addToNameService(pVideo, params_.name.c_str());
  }
  
  VideoImpl::~VideoImpl()
  {
    cout << "Destructing VideoImpl." << endl;

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
  
  Miro::ImageHandleIDL *
  VideoImpl::connect(CORBA::ULong& _id) ACE_THROW_SPEC(())
  {
    _id = ++idCounter;
    return new Miro::ImageHandleIDL(imageHandle_);
  }

  void
  VideoImpl::disconnect(CORBA::ULong /*_id*/) 
    ACE_THROW_SPEC((EOutOfBounds))
  {
  }

  TimeIDL
  VideoImpl::acquireCurrentImage(CORBA::ULong /*_id*/, CORBA::ULong& _buffer) 
    ACE_THROW_SPEC((EOutOfBounds))
  {
    TimeIDL stamp;

    _buffer = pBufferManager_->acquireCurrentReadBuffer();
    timeA2C(pBufferManager_->bufferTimeStamp(_buffer), stamp);

    cout << "got current image" << endl;

    return stamp;
  }

  TimeIDL
  VideoImpl::acquireNextImage(CORBA::ULong /*_id*/, CORBA::ULong& _buffer) 
    ACE_THROW_SPEC((EOutOfBounds, ETimeOut))
  {
    TimeIDL stamp;

    cout << "wait for next image" << endl;

    _buffer = pBufferManager_->acquireNextReadBuffer();
    timeA2C(pBufferManager_->bufferTimeStamp(_buffer), stamp);

    cout << "got next image" << endl;

    return stamp;
  }

  void
  VideoImpl::releaseImage(CORBA::ULong /*_id*/, CORBA::ULong _buffer)
    ACE_THROW_SPEC((EOutOfBounds))
  {
    pBufferManager_->releaseReadBuffer(_buffer);
    cout << "released image" << endl;
  }

  SubImageDataIDL *
  VideoImpl::exportWaitSubImage(CORBA::ULong& x, CORBA::ULong& y)
    ACE_THROW_SPEC((EOutOfBounds, EDevIO, ETimeOut))
  {
    int             bufferSize = ::Miro::getImageSize(format_);
    unsigned char   * dst   = new unsigned char[bufferSize];
    SubImageDataIDL * subImage = new SubImageDataIDL(bufferSize, bufferSize, dst, 1);

    unsigned int bufferId = pBufferManager_->acquireNextReadBuffer();
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
};
