// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "VideoHelper.h"
#include "Exception.h"

// #include <ace/Shared_Memory.h>

#include <sys/shm.h>

#include <cmath>

namespace Miro
{
  unsigned int 
  getPixelSize(VideoPaletteIDL _pal)
  {
    unsigned int pixelSize = 0;

    switch (_pal)
    {
    case GREY_8:	//	gray
    case INVALID:	//	dummy color model for filters not producing a meaningful output image
      pixelSize = 1;
      break;
    case GREY_16:	//	gray
      pixelSize = 2;
      break;
    case GREY_32:	//	gray
      pixelSize = 4;
      break;
    case YUV_24:  //      yuv24:
    case RGB_24:	//	rgb24:
    case BGR_24:	//	bgr24:
    case HSV_24:		//	hsv24
      pixelSize = 3;
      break;
    case LUV_24:        //luv24
      pixelSize = 3;
      break;
    case BGR_32:	//	rgb32:
    case RGB_32:	//	bgr32:
      pixelSize = 4;
      break;
    default:
      throw Exception("VideoDevice::getImageSize: unknown palette.\n");
      break;
    }
    return pixelSize;
  }
  
  unsigned int
  getImageSize(ImageFormatIDL const& _format)
  {
    unsigned int imageSize = 0;
    if (_format.palette == YUV_422)
      imageSize = _format.width * _format.height * 2;
    else if (_format.palette == YUV_411)
      imageSize = (unsigned int)
	ceil((double)(_format.width * _format.height * 3) / 2.);
//    else if (_format.palette == YUV_420P)
//      imageSize = _format.width * 3 * (_format.height / 2);
    else
      imageSize = _format.width * _format.height * getPixelSize(_format.palette);
    return imageSize;
  }

  unsigned int
  getRowSize(ImageFormatIDL const& _format)
  {
    unsigned int rowSize = 0;
    if (_format.palette == YUV_422)
      rowSize = _format.width * 2;
    else if (_format.palette == YUV_411)
      rowSize = (unsigned int)
	ceil((double)(_format.width * 3) / 2.);
//    else if (_format.palette == YUV_420P)
//      rowSize = _format.width * 2;
    else
      rowSize = _format.width * getPixelSize(_format.palette);
    return rowSize;
  }

  VideoConnection::VideoConnection(Video_ptr _video) :
    video(Video::_duplicate(_video)),
    id(0),
    memory(NULL)
  {
    handle = video->connect(id);
    memory = (unsigned char*)::shmat(handle->key, NULL, 0);
    if ((long)memory == -1)
      throw CException(errno, "Unable to map shared memory.");
  }

  VideoConnection::~VideoConnection()
  {
    ::shmdt((void*)memory);
    video->disconnect(id);
  }
};
