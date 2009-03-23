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
#include "VideoFilterBasic.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(FilterCopy);
  FILTER_PARAMETERS_FACTORY_IMPL(FilterSwap3);
  FILTER_PARAMETERS_FACTORY_IMPL(FilterSwap4);
  FILTER_PARAMETERS_FACTORY_IMPL(FilterFlip);
  FILTER_PARAMETERS_FACTORY_IMPL(FilterReverse);

  FilterCopy::FilterCopy(const Miro::ImageFormatIDL& _format) :
    Super(_format),
    imageSize_(getImageSize(_format))
  {
  }

  void
  FilterCopy::process()
  {
     memcpy(outputBuffer(), inputBuffer(), imageSize_);
  }

  FilterSwap3::FilterSwap3(const Miro::ImageFormatIDL& _format) :
    Super(_format),
    imageSize_(getImageSize(_format))
  {
    inplace_ = true;

    if (inputFormat_.palette != Miro::RGB_24 &&
	inputFormat_.palette != Miro::BGR_24)
      throw Miro::Exception("Incompatible input format for FilterYUV422toYUV.");

    if (inputFormat_.palette == Miro::RGB_24)
      outputFormat_.palette = Miro::BGR_24;
    if (inputFormat_.palette == Miro::BGR_24)
      outputFormat_.palette = Miro::RGB_24;
  }

  void
  FilterSwap3::process()
  {
    unsigned char const * first = inputBuffer();
    unsigned char const * last = inputBuffer() + imageSize_;
    unsigned char * dest = outputBuffer();
    char r, g, b;
    while (first < last) {
      b = *first++;
      g = *first++;
      r = *first++;
      *dest++ = r;
      *dest++ = g;
      *dest++ = b;
    }
  }

  FilterSwap4::FilterSwap4(const Miro::ImageFormatIDL& _format) :
    Super(_format),
    imageSize_(getImageSize(_format))
  {
    inplace_ = true;

    if (inputFormat_.palette != Miro::RGB_32 &&
	inputFormat_.palette != Miro::BGR_32)
      throw Miro::Exception("Incompatible input format for FilterYUV422toYUV.");

    if (inputFormat_.palette == Miro::RGB_32)
      outputFormat_.palette = Miro::BGR_32;
    if (inputFormat_.palette == Miro::BGR_32)
      outputFormat_.palette = Miro::RGB_32;
}

  void
  FilterSwap4::process()
  {
    unsigned int const * first = 
      reinterpret_cast<unsigned int const *>(inputBuffer());
    unsigned int const * last =
      reinterpret_cast<unsigned int const *>(inputBuffer() + imageSize_);
    unsigned int * dest = 
      reinterpret_cast<unsigned int *>(outputBuffer());
    while (first < last) {
      *(dest++) = __bswap_32(*(first++));
    }
  }

  FilterFlip::FilterFlip(const Miro::ImageFormatIDL& _format) :
    Super(_format),
    rowSize_(getRowSize(_format))
  {
  }

  void
  FilterFlip::process()
  {
    unsigned char const * src = inputBuffer();
    unsigned char * dst = outputBuffer() + rowSize_ * (inputFormat_.height - 1);
    for (unsigned int i = inputFormat_.height; i != 0; --i) {
      memcpy(dst, src, rowSize_);
      src += rowSize_;
      dst -= rowSize_;
    }
  }

  FilterReverse::FilterReverse(const Miro::ImageFormatIDL& _format) :
    Super(_format),
    rowSize_(getRowSize(_format))
  {
    switch(inputFormat_.palette)
      {
      case Miro::GREY_8: 
	bytesPerPixel = 1;
	break;
      case Miro::GREY_16:
	bytesPerPixel = 2;
	break;
      case Miro::GREY_32:
	bytesPerPixel = 4;
	break;
      case Miro::RGB_24:
	bytesPerPixel = 3;
	break;
      case Miro::BGR_24:
	bytesPerPixel = 3;
	break;
      case Miro::RGB_32:
	bytesPerPixel = 4;
	break;
      case Miro::BGR_32:
	bytesPerPixel = 4;
	break;
      default:
	MIRO_LOG(LL_CRITICAL, "cannot handle YUV");
	exit(-1);
      }
  }

  void
  FilterReverse::process()
  {
    unsigned char const * src = inputBuffer();
    unsigned char * dst = outputBuffer() + rowSize_ * (inputFormat_.height - 1);
    int numberOfPixel = inputFormat_.height * inputFormat_.width;

    for (unsigned int i = numberOfPixel; i != 0; --i) 
    {
      memcpy(dst, src, bytesPerPixel);
      src += bytesPerPixel;
      dst -= bytesPerPixel;
    }
  }

  FILTER_PARAMETERS_FACTORY_IMPL(FilterHalfImage);

  FilterHalfImage::FilterHalfImage(const Miro::ImageFormatIDL& _format) :
    Super(_format),
    rowSize_(getRowSize(_format)),
    rowSize2_(rowSize_ * 2),
    offset_(0)
  {
    outputFormat_.height /= 2;
  }

  void
  FilterHalfImage::init(Miro::Server& _server, FilterParameters const * _params)
  {
    Super::init(_server, _params);
    
    FilterHalfImageParameters const *
      params = dynamic_cast<FilterHalfImageParameters const *>(_params);
    assert(params != NULL);

    if (params->odd)
      offset_ = rowSize_;
  }

  void
  FilterHalfImage::process()
  {
    unsigned char const * src = inputBuffer() + offset_;
    unsigned char * dst = outputBuffer();

    for (unsigned int i = 0; i < inputFormat_.height; i += 2) {
      memcpy(dst, src, rowSize_);
      src += rowSize2_;
      dst += rowSize_;
    }
  }
}
