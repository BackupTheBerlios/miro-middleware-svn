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
#include "VideoImpl.h"
#include "VideoDevice.h"
#include "VideoConfig.h"

#include <miro/Exception.h>

#include <sys/ipc.h>
#include <sys/shm.h>

using namespace Video;

namespace Miro 
{
  VideoImpl::VideoImpl(::Video::Consumer* _pconsumer) :
    pConsumer(_pconsumer), 
    parameters(*Parameters::instance())
  {
    cout << "Constructing VideoImpl." << endl;

    iMaxConnections = parameters.connections;
    pHandleArray = new ImageHandleIDL[iMaxConnections];
    pShmDataArray = new void*[iMaxConnections];

    for (int i = 0; i < iMaxConnections; i++)
    {
      pHandleArray[i].width = parameters.width;
      pHandleArray[i].height = parameters.height;
      if (parameters.palette == "gray") {
	pHandleArray[i].palette = ::Video::paletteGrey;
      } 
      else if (parameters.palette == "yuv") {
	pHandleArray[i].palette = ::Video::paletteYUV;
      }
      else if (parameters.palette == "rgb") {
	pHandleArray[i].palette = ::Video::paletteRGB;
      } 
      else if (parameters.palette == "bgr") {
	pHandleArray[i].palette = ::Video::paletteBGR;
      } 
      else if (parameters.palette == "rgba") {
	pHandleArray[i].palette = ::Video::paletteRGBA;
      }
      else if (parameters.palette == "abgr") {
	pHandleArray[i].palette = ::Video::paletteABGR;
      }
      //	pHandleArray[i].source = parameters.source;
      pHandleArray[i].key = -1;
      pHandleArray[i].handle = -1;
      pShmDataArray[i] = NULL;
    }
  }

  VideoImpl::~VideoImpl()
  {
    cout << "Destructing VideoImpl." << endl;

    delete[] pShmDataArray;
    delete[] pHandleArray;
  }
	
  Miro::ImageHandleIDL 
  VideoImpl::connect() throw()
  {
    for (int i = 0; i < iMaxConnections; ++i) {
      if (pHandleArray[i].handle == -1) {
	pHandleArray[i].key = shmget(0, pConsumer->getImageSize(), IPC_CREAT|0x1ff);
	if (pHandleArray[i].key == -1) {
	  cout << "failed creating shared memory segment!" << endl;
	  throw Miro::EOutOfBounds();
	}

	pShmDataArray[i] = shmat(pHandleArray[i].key, 0, 0);
	if (pShmDataArray[i] == (void*)-1) {
	  cout << "failed attaching shared memory segment!" << endl;
	  throw Miro::EOutOfBounds();
	}

	pHandleArray[i].handle = i;
	return pHandleArray[i];
      }
    }
    throw Miro::EOutOfBounds();
  }


  void 
  VideoImpl::release(const Miro::ImageHandleIDL & img) throw()
  {
    checkImageHandle(img);
    shmdt(pShmDataArray[img.handle]);
    shmctl(pHandleArray[img.handle].key, IPC_RMID, NULL);
    pHandleArray[img.handle].key = -1;
    pHandleArray[img.handle].handle = -1;
  }

  TimeIDL
  VideoImpl::getImage(Miro::ImageHandleIDL & img) throw()
  {
    checkImageHandle(img);
    try {
      return pConsumer->getCurrentImage(pShmDataArray[img.handle]);
    }
    catch (...) {
      throw Miro::EDevIO();
    }
  }

  TimeIDL
  VideoImpl::getWaitImage(Miro::ImageHandleIDL & img) throw()
  {
      cout << "getWaitImage 0" << endl;
    checkImageHandle(img);
      cout << "getWaitImage 1" << endl;
      try {
      cout << "getWaitImage 2" << endl;
      return pConsumer->getWaitNextImage(pShmDataArray[img.handle]);
    }
    catch (Exception& e) {
      cout << "VideoImpl::getWaitImage() caught exception: " << e << endl;
      throw Miro::EDevIO();
    }
  }

  SubImageDataIDL *
  VideoImpl::exportWaitSubImage (CORBA::Long x, CORBA::Long y)
    ACE_THROW_SPEC ((CORBA::SystemException, Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut))
  {
    int             bufferSize = pConsumer->getPaletteSize() * (int)x * (int)y;
    unsigned char   * buffer   = new unsigned char[bufferSize];
    SubImageDataIDL * subImage = new SubImageDataIDL(bufferSize, bufferSize, buffer, 1);

    try {
      pConsumer->getWaitNextSubImage(buffer, (int)x, (int)y);
    }
    catch (...) {
      throw Miro::EDevIO();
    }
    return subImage;
  }

  void 
  VideoImpl::checkImageHandle(const Miro::ImageHandleIDL & img)
  {
    if ((img.handle < 0) || 
	(img.handle >= iMaxConnections) || 
	(pHandleArray[img.handle].key == -1) ||
	(img.key != pHandleArray[img.handle].key) || 
	(pShmDataArray[img.handle] == NULL))
      throw Miro::EOutOfBounds();
  }
};
