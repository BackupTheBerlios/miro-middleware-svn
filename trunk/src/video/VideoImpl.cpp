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
#include <sys/ipc.h>
#include <sys/shm.h>

#include <miro/Exception.h>
#include "video/VideoImpl.h"
#include "video/VideoDevice.h"

using namespace Video;

namespace Miro 
{
  VideoImpl::VideoImpl(::Video::Consumer* _pconsumer) :
    pConsumer(_pconsumer), 
    parameters(*Parameters::instance())
  {
	int	i;
	
	iMaxConnections = parameters.connections;
  	pHandleArray = new ImageHandleIDL[iMaxConnections];
  	pShmDataArray = new void*[iMaxConnections];
	for (i=0; i<iMaxConnections; i++)
		{
		pHandleArray[i].width = parameters.width;
		pHandleArray[i].height = parameters.height;
		if (parameters.palette=="gray") {
		  pHandleArray[i].palette = paletteGrey;
		} else if (parameters.palette=="rgb") {
		  pHandleArray[i].palette = paletteRGB;
		} else if (parameters.palette=="bgr") {
		  pHandleArray[i].palette = paletteBGR;
		} else if (parameters.palette=="rgba") {
		  pHandleArray[i].palette = paletteRGBA;
		} else if (parameters.palette=="abgr") {
		  pHandleArray[i].palette = paletteABGR;
		}
		//	pHandleArray[i].source = parameters.source;
		pHandleArray[i].key = -1;
		pHandleArray[i].handle = -1;
		pShmDataArray[i] = NULL;
		}
	}

VideoImpl::~VideoImpl()
	{
	delete[] pShmDataArray;
	delete[] pHandleArray;
	}
	
  Miro::ImageHandleIDL VideoImpl::connect() throw()
	{
	int	i;
	
	for (i=0; i<iMaxConnections; i++)
		{
		if (pHandleArray[i].handle == -1)
			{
			pHandleArray[i].key = shmget(0, pConsumer->getImageSize(), IPC_CREAT|0x1ff);
			if (pHandleArray[i].key == -1)
				{
				cout << "failed creating shared memory segment!" << endl;
				throw Miro::EOutOfBounds();
				}
			pShmDataArray[i] = shmat(pHandleArray[i].key, 0, 0);
			if (pShmDataArray[i] == (void*)-1)
				{
				cout << "failed attaching shared memory segment!" << endl;
				throw Miro::EOutOfBounds();
				}
			pHandleArray[i].handle = i;
			return pHandleArray[i];
			}
		}
	throw Miro::EOutOfBounds();
	}


void VideoImpl::release(const Miro::ImageHandleIDL & img) throw()
	{
	checkImageHandle(img);
	shmdt(pShmDataArray[img.handle]);
	shmctl(pHandleArray[img.handle].key, IPC_RMID, NULL);
	pHandleArray[img.handle].key = -1;
	pHandleArray[img.handle].handle = -1;
	}

void VideoImpl::getImage(Miro::ImageHandleIDL & img) throw()
	{
	checkImageHandle(img);
	try
	  {
	  pConsumer->getCurrentImage(pShmDataArray[img.handle]);
	  }
	catch (...)
	  {
	  throw Miro::EDevIO();
	  }
	}

void VideoImpl::getWaitImage(Miro::ImageHandleIDL & img) throw()
	{
	checkImageHandle(img);
	try
	  {
	  pConsumer->getWaitNextImage(pShmDataArray[img.handle]);
	  }
	catch (Exception& e)
	  {
	cout << "VideoImpl::getWaitImage() caught exception: " << e << endl;
	  throw Miro::EDevIO();
	  }
	}

void VideoImpl::checkImageHandle(const Miro::ImageHandleIDL & img)
	{
	if ((img.handle < 0) || (img.handle >= iMaxConnections) || (pHandleArray[img.handle].key == -1) ||
	    (img.key != pHandleArray[img.handle].key) || (pShmDataArray[img.handle] == NULL))
		throw Miro::EOutOfBounds();
	}
};
