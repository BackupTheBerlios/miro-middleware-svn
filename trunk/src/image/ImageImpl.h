// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ImageImpl_h
#define ImageImpl_h

#include <termios.h>

#include <ace/Event_Handler.h>
#include <ace/Synch.h>

#include <exception>
#include <string>

#include "miro/OdometryC.h"
#include "miro/ImageS.h"

#include "ImageFactory.h"

namespace Miro {

/**
* ImageImpl
*/
class ImageImpl :  public virtual POA_Miro::Image
	{
	public:
		ImageImpl(bool &positionStamps_,
		     Odometry_ptr);
		virtual	~ImageImpl();

		virtual Miro::ImageIDL requestImage (
			CORBA::Long w,
			CORBA::Long h,
			CORBA::Long formats,
			CORBA::Long tolerance,
			CORBA::Long source,
			CORBA::Long subfields
			) throw(Miro::EOutOfBounds, Miro::EDevIO);
	
		virtual Miro::ImageIDL requestLeftImage (
			CORBA::Long w,
			CORBA::Long h,
			CORBA::Long formats,
			CORBA::Long tolerance,
			CORBA::Long source,
			CORBA::Long subfields
			) throw(Miro::EOutOfBounds, Miro::EDevIO);
	
		virtual Miro::ImageIDL requestRightImage (
			CORBA::Long w,
			CORBA::Long h,
			CORBA::Long formats,
			CORBA::Long tolerance,
			CORBA::Long source,
			CORBA::Long subfields
			) throw(Miro::EOutOfBounds, Miro::EDevIO);
	
		virtual Miro::StereoImageIDL requestStereoImage (
			CORBA::Long w,
			CORBA::Long h,
			CORBA::Long formats,
			CORBA::Long tolerance,
			CORBA::Long source,
			CORBA::Long subfields
			) throw(Miro::EOutOfBounds, Miro::EDevIO);
	

		virtual void getImage (
			Miro::ImageIDL & img
			) throw(Miro::EOutOfBounds, Miro::EDevIO);
	
		virtual void getStereoImage (
			Miro::StereoImageIDL & img
			) throw(Miro::EOutOfBounds, Miro::EDevIO);
	

		virtual void getWaitImage (
			Miro::ImageIDL & img
			) throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut);
	
		virtual void getWaitStereoImage (
			Miro::StereoImageIDL & img
			) throw(Miro::EOutOfBounds, Miro::EDevIO, Miro::ETimeOut);
	

		virtual void releaseImage (
			Miro::ImageIDL & img
			) throw(Miro::EOutOfBounds);
	
		virtual void releaseStereoImage (
			Miro::StereoImageIDL & img
			) throw(Miro::EOutOfBounds);

		virtual Miro::ImageDescriptionIDL getImageDescription () throw();
	
	private:
		MiroImage::ImageFactory* imageFactory;
  };
};
#endif




