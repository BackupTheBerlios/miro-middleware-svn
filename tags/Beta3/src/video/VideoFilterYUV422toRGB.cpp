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

/*! \file VideoFilterYUV422toRGB.cpp
 *  \brief Image conversion routines YUV->RGB
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilterYUV422toRGB.h"

#include "miro/VideoHelper.h"
#include <miro/Exception.h>

namespace Video
{
  //---------------------------------------------------------------
  FilterYUV422toRGB::FilterYUV422toRGB(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    if (_inputFormat.palette != Miro::YUV_422)
      throw Miro::Exception("Incompatible input format for FilterYUV422toRGB.");
    outputFormat_.palette = Miro::RGB_24;
  }

  //---------------------------------------------------------------
  FilterYUV422toRGB::~FilterYUV422toRGB()
  {
  }

  //---------------------------------------------------------------
  void 
  FilterYUV422toRGB::process()
  {
    unsigned char const * src_img = inputBuffer();
    unsigned char * tgt_img = outputBuffer();
    unsigned int srcImgSize = getImageSize(inputFormat_);

    for (unsigned int i = 0; i < srcImgSize; i += 4) {
      register int u  = src_img[i];
      register int y0 = src_img[i+1];
      register int v  = src_img[i+2];
      register int y1 = src_img[i+3];

      *(tgt_img++) = t_r[(y0<<8)|v];
      *(tgt_img++) = t_g2[(y0<<8)|t_g1[(u<<8)|v]];
      *(tgt_img++) = t_b[(y0<<8)|u];
      
      *(tgt_img++) = t_r[(y1<<8)|v];
      *(tgt_img++) = t_g2[(y1<<8)|t_g1[(u<<8)|v]];
      *(tgt_img++) = t_b[(y1<<8)|u];
    }
  }
};

