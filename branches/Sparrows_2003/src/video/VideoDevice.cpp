// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "VideoDevice.h"

#include "miro/Exception.h"

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------
  VideoDevice::VideoDevice(Parameters const * _params) :
    params_(_params),
    requestedPaletteID(0),
    devicePaletteID(0),
    requestedSubfieldID(0),
    deviceSubfieldID(0),
    imgWidth(0),
    imgHeight(0)
  {
  }

  //--------------------------------------------------------------------
  VideoDevice::~VideoDevice()
  {
  }

  //--------------------------------------------------------------------
  int
  VideoDevice::getDevicePalette() const
  {
    return devicePaletteID;
  }

  //--------------------------------------------------------------------
  int
  VideoDevice::getRequestedPalette() const
  {
    return requestedPaletteID;
  }

  //--------------------------------------------------------------------
  int
  VideoDevice::getDeviceSubfield() const
  {
    return deviceSubfieldID;
  }

  //--------------------------------------------------------------------
  int
  VideoDevice::getRequestedSubfield() const
  {
    return requestedSubfieldID;
  }

 
  //--------------------------------------------------------------------
  int
  VideoDevice::getImageWidth() const
  {
    return imgWidth;
  }

  //--------------------------------------------------------------------
  int
  VideoDevice::getImageHeight() const
  {
    return imgHeight;
  }

  //--------------------------------------------------------------------
  int
  VideoDevice::getImageSize() const
  {
    return imgWidth * imgHeight * getPixelSize(requestedPaletteID);
  }

  //--------------------------------------------------------------------
  int
  VideoDevice::getDeviceImageSize() const
  {
    return imgWidth * imgHeight * getPixelSize(devicePaletteID);
  }


  //--------------------------------------------------------------------
  int
  VideoDevice::getPixelSize(const int pal) const
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
      throw Miro::Exception("VideoDevice::getImageSize: unknown palette.\n");
      break;
    }
    return pixelSize;
  }
};

