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


#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Timer_Queue.h>

#include "ImageImpl.h"
//	#include "ImageDescription.h"

//	#include <termios.h>
//	#include <linux/serial.h>

#include <iostream>
#include <cmath>

#include "ImageFactory.h"
// #include "miro/Exception.h"

using namespace Miro;

#undef DEBUG

/***************************************************************************
*
*  Image Implementation 
*
***************************************************************************/

ImageImpl::ImageImpl( bool &,
		     Odometry_ptr)
       :imageFactory(NULL)
	{
	imageFactory = new MiroImage::ImageFactory;
	}



ImageImpl::~ImageImpl()
	{
	if (imageFactory)
		delete imageFactory;
	}

Miro::ImageIDL ImageImpl::requestImage (CORBA::Long w, CORBA::Long h, CORBA::Long formats,
										CORBA::Long tolerance, CORBA::Long source, CORBA::Long subfields)
										throw(Miro::EOutOfBounds, Miro::EDevIO)
	{
	Miro::ImageIDL	miroImg;
	
	if (imageFactory)
		miroImg = imageFactory->requestImage(w, h, formats, tolerance, source, subfields);
	return miroImg;
	}

Miro::ImageIDL ImageImpl::requestLeftImage (CORBA::Long w, CORBA::Long h, CORBA::Long formats,
										CORBA::Long tolerance, CORBA::Long source, CORBA::Long subfields)
										throw(Miro::EOutOfBounds, Miro::EDevIO)
	{
	Miro::ImageIDL	miroImg;
	
	if (imageFactory)
		miroImg = imageFactory->requestLeftImage(w, h, formats, tolerance, source, subfields);
	return miroImg;
	}

Miro::ImageIDL ImageImpl::requestRightImage (CORBA::Long w, CORBA::Long h, CORBA::Long formats,
										CORBA::Long tolerance, CORBA::Long source, CORBA::Long subfields)
										throw(Miro::EOutOfBounds, Miro::EDevIO)
	{
	Miro::ImageIDL	miroImg;
	
	if (imageFactory)
		miroImg = imageFactory->requestRightImage(w, h, formats, tolerance, source, subfields);
	return miroImg;
	}

Miro::StereoImageIDL ImageImpl::requestStereoImage (CORBA::Long w, CORBA::Long h, CORBA::Long formats,
										CORBA::Long tolerance, CORBA::Long source, CORBA::Long subfields)
										throw(Miro::EOutOfBounds, Miro::EDevIO)
	{
	Miro::StereoImageIDL	miroImg;
	
	if (imageFactory)
		miroImg = imageFactory->requestStereoImage(w, h, formats, tolerance, source, subfields);
	return miroImg;
	}
	

void ImageImpl::getImage(Miro::ImageIDL & img) throw(Miro::EOutOfBounds, Miro::EDevIO)
	{
	if (imageFactory)
		imageFactory->updateImage(img);
	}

void ImageImpl::getStereoImage(Miro::StereoImageIDL & img) throw(Miro::EOutOfBounds, Miro::EDevIO)
	{
	if (imageFactory)
		imageFactory->updateStereoImage(img);
	}
	

void ImageImpl::getWaitImage(Miro::ImageIDL & img) throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut)
	{
	if (imageFactory)
		imageFactory->updateImage(img);
	}

void ImageImpl::getWaitStereoImage(Miro::StereoImageIDL & img) throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut)
	{
	if (imageFactory)
		imageFactory->updateStereoImage(img);
	}
	

void ImageImpl::releaseImage(Miro::ImageIDL & img) throw(Miro::EOutOfBounds)
	{
	if (imageFactory)
		imageFactory->releaseImage(img);
	}

void ImageImpl::releaseStereoImage(Miro::StereoImageIDL & img) throw(Miro::EOutOfBounds)
	{
	if (imageFactory)
		imageFactory->releaseStereoImage(img);
	}


Miro::ImageDescriptionIDL ImageImpl::getImageDescription() throw()
	{
	Miro::ImageDescriptionIDL	description;
	
	return description;
	}
