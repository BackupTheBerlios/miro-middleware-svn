// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#include "Pixels.h"
#include "patternSource.h"

namespace MiroImage 
{

  patternSource::patternSource()
  {
    iNRequests = 0;
    iLeftOffset = 0;
    iRightOffset = 0;
  }

  patternSource::~patternSource()
  {
#ifdef DEBUG
    if (iNRequests != 0)
      std::cerr << "~patternSource(): iNRequests != 0, iNRequests == " << iNRequests << endl;
#endif
  }
		
  void patternSource::requestImage(ImageIDL& img, long /* tolerance */)
  {
    integerType	ix, iy;
    rgb24		forePixel;
    rgb24		backPixel;
	
    if ((img.dataSubfields == kEvenSubfield) || (img.dataSubfields == kOddSubfield))
    {
      if (img.dataChannel & kLeftChannel)
      {
	leftMapStruct = img;
	leftMapStruct.height = 2 * leftMapStruct.height;
	leftMap = new rgb24[leftMapStruct.width * leftMapStruct.height];
      }
      if (img.dataChannel & kRightChannel)
      {
	rightMapStruct = img;
	rightMapStruct.height = 2 * rightMapStruct.height;
	rightMap = new rgb24[rightMapStruct.width * rightMapStruct.height];
      }
    }
    else
    {
      if (img.dataChannel & kLeftChannel)
      {
	leftMapStruct = img;
	leftMap = new rgb24[leftMapStruct.width * leftMapStruct.height];
      }
      if (img.dataChannel & kRightChannel)
      {
	rightMapStruct = img;
	rightMap = new rgb24[rightMapStruct.width * rightMapStruct.height];
      }
    }
	
    rgb24*	pData;

    if (img.dataChannel & kLeftChannel)
    {
      pData = leftMap;
      for (iy = 0; iy < leftMapStruct.height; iy++)
	for (ix = 0; ix < leftMapStruct.width; ix++)
	{
	  if (((ix/64) % 2) && ((iy/64) % 2))
	  {
	    forePixel.r = ix/2;
	    forePixel.g = 255 - ix/4;
	    forePixel.b = 255 - ix/8;

	    *pData++ = forePixel;
	  }
	  else
	  {
	    backPixel.r = 255 - ix/2;
	    backPixel.g = ix/4;
	    backPixel.b = ix/8;

	    *pData++ = backPixel;
	  }
	}
    }

    if (img.dataChannel & kRightChannel)
    {
      pData = rightMap;
      for (iy = 0; iy < rightMapStruct.height; iy++)
	for (ix = 0; ix < rightMapStruct.width; ix++)
	{
	  if (((ix/64) % 2) && ((iy/64) % 2))
	  {
	    forePixel.r = ix/2;
	    forePixel.g = 255 - ix/4;
	    forePixel.b = 255 - ix/8;

	    *pData++ = forePixel;
	  }
	  else
	  {
	    backPixel.r = 255 - ix/2;
	    backPixel.g = ix/4;
	    backPixel.b = ix/8;

	    *pData++ = backPixel;
	  }
	}
    }
    iNRequests = iNRequests + 1;
  }

  void patternSource::releaseImage(ImageIDL& img)
  {
    if (iNRequests >= 0)
    {
      iNRequests--;
      if (iNRequests <= 0)
      {
	if (leftMap && (img.dataChannel & kLeftChannel))
	  delete[] leftMap;
	if (img.dataChannel & kRightChannel)
	  delete[] rightMap;
	iNRequests = 0;
      }
    }
  }

  void patternSource::getLeftImage(ImageIDL& img)
  {
    integerType		ix, iy;
    integerType		lineOffset;
    rgb24*			pDataSrc = (rgb24*)leftMap;
	
    if (img.dataSubfields == kEvenSubfield)
    {
      lineOffset = img.width;
    }
    else if (img.dataSubfields == kOddSubfield)
    {
      pDataSrc += img.width;
      lineOffset = img.width;
    }
    else
    {
      lineOffset = 0;
    }

    if (((img.dataFormats & kRGB) == kRGB) && img.rgbImage.sharedMemoryValid)
    {
      rgb24*	pRGB = (rgb24*)img.rgbImage.serverShMemData;
		
      for (iy = 0; iy < img.height; iy++)
      {
	for (ix = 0; ix < img.width; ix++)
	  *pRGB++ = *pDataSrc++;
	pDataSrc += lineOffset;
      }
		
      img.rgbImage.dataValid = true;
      img.sourceFormat = kRGB;
    }
    else if (((img.dataFormats & kHSV) == kHSV) && img.hsvImage.sharedMemoryValid)
    {
      hsv32*	pHSV = (hsv32*)img.hsvImage.serverShMemData;
		
      for (iy = 0; iy < img.height; iy++)
      {
	for (ix = 0; ix < img.width; ix++)
	{
	  pixelRGB32behind24LE	p(*pDataSrc++);
	  hsv32::rgb2hsv(*pHSV++, p.r, p.g, p.b);
	}
	pDataSrc += lineOffset;
      }

      img.hsvImage.dataValid = true;
      img.sourceFormat = kHSV;
    }
    else if (((img.dataFormats & kGray) == kGray) && img.grayImage.sharedMemoryValid)
    {
      gray*	pGray = (gray*)img.grayImage.serverShMemData;
		
      for (iy = 0; iy < img.height; iy++)
      {
	for (ix = 0; ix < img.width; ix++)
	  *pGray++ = *pDataSrc++;
	pDataSrc += lineOffset;
      }

      img.grayImage.dataValid = true;
      img.sourceFormat = kGray;
    }
	
    iLeftOffset = (iLeftOffset + 4) % 64;
  }

  void patternSource::getRightImage(ImageIDL& img)
  {
    integerType		ix, iy;
    integerType		lineOffset;
    rgb24*			pDataSrc = (rgb24*)rightMap;
	
    if (img.dataSubfields == kEvenSubfield)
    {
      lineOffset = img.width;
    }
    else if (img.dataSubfields == kOddSubfield)
    {
      pDataSrc += img.width;
      lineOffset = img.width;
    }
    else
      lineOffset = 0;
		
    if (((img.dataFormats & kRGB) == kRGB) && img.rgbImage.sharedMemoryValid)
    {
      rgb24*	pRGB = (rgb24*)img.rgbImage.serverShMemData;
		
      for (iy = 0; iy < img.height; iy++)
      {
	for (ix = 0; ix < img.width; ix++)
	  *pRGB++ = *pDataSrc++;
	pDataSrc += lineOffset;
      }

      img.rgbImage.dataValid = true;
      img.sourceFormat = kRGB;
    }
    else if (((img.dataFormats & kHSV) == kHSV) && img.hsvImage.sharedMemoryValid)
    {
      hsv32*	pHSV = (hsv32*)img.hsvImage.serverShMemData;
		
      for (iy = 0; iy < img.height; iy++)
      {
	for (ix = 0; ix < img.width; ix++)
	{
	  pixelRGB32behind24LE	p(*pDataSrc++);
	  hsv32::rgb2hsv(*pHSV++, p.r, p.g, p.b);
	}
	pDataSrc += lineOffset;
      }

      img.hsvImage.dataValid = true;
      img.sourceFormat = kHSV;
    }
    else if (((img.dataFormats & kGray) == kGray) && img.grayImage.sharedMemoryValid)
    {
      gray*	pGray = (gray*)img.grayImage.serverShMemData;
		
      for (iy = 0; iy < img.height; iy++)
      {
	for (ix = 0; ix < img.width; ix++)
	  *pGray++ = *pDataSrc++;
	pDataSrc += lineOffset;
      }

      img.grayImage.dataValid = true;
      img.sourceFormat = kGray;
    }
	
    iRightOffset = (iRightOffset + 4) % 64;
  }

}
