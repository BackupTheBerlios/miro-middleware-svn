// -*- c++ -*- ///////////////////////////////////////////////////////////////
/*! \file VideoDeviceBase.cpp
 *  \brief Base class for a video device
 *
 * -----------------------------------------------------
 *
 * RoboCup Middle Size Team, Technische Universitaet Graz
 * "Mostly Harmless"
 * http://www.robocup.tugraz.at
 *
 * Project: Miro::Video
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 * $Log$
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.2  2003/03/08 09:24:36  muehlenf
 * Now Video::Parameters are passed through to VideoDevice.
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <miro/Exception.h>
#include "VideoDeviceBase.h"

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
//--------------------------------------------------------------------
  VideoDeviceBase::VideoDeviceBase() :
    params_(Parameters::instance()),
      formatID(0),
        sourceID(0),
	requestedPaletteID(0),
	devicePaletteID(0),
	requestedSubfieldID(0),
	deviceSubfieldID(0),
	imgWidth(0),
	imgHeight(0)
    {
    }

//--------------------------------------------------------------------
    VideoDeviceBase::~VideoDeviceBase()
    {
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::countBuffers() const
    {
 	return iNBuffers;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::countFramesCaptured() const
    {
 	return iNFramesCaptured;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getFormat() const
    {
  	return formatID;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getSource()  const
    {
	return sourceID;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getDevicePalette() const
    {
	return devicePaletteID;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getRequestedPalette() const
    {
	return requestedPaletteID;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getDeviceSubfield() const
    {
	return deviceSubfieldID;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getRequestedSubfield() const
    {
	return requestedSubfieldID;
    }

 
//--------------------------------------------------------------------
    int VideoDeviceBase::getImageWidth() const
    {
 	return imgWidth;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getImageHeight() const
    {
 	return imgHeight;
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getImageSize() const
    {
	return imgWidth * imgHeight * getPixelSize(requestedPaletteID);
    }

//--------------------------------------------------------------------
    int VideoDeviceBase::getDeviceImageSize() const
    {
	return imgWidth * imgHeight * getPixelSize(devicePaletteID);
    }


//--------------------------------------------------------------------
    int VideoDeviceBase::getPixelSize(const int pal) const
    {
	int	pixelSize = 0;

	switch (pal)
	{
	    case paletteGrey:	//	gray
		pixelSize = 1;
		break;
	    case paletteYUV:    //      yuv24:
	    case paletteRGB:	//	rgb24:
	    case paletteBGR:	//	bgr24:
		pixelSize = 3;
		break;
	    case paletteRGBA:	//	rgb32:
	    case paletteABGR:	//	bgr32:
		pixelSize = 4;
		break;
	    default:
		cout << "VideoDeviceBase::getImageSize: unknown palette" << endl;
		break;
	}
	return pixelSize;
    }
};


