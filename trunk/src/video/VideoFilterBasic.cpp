// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "VideoFilterBasic.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

namespace Video
{
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
};