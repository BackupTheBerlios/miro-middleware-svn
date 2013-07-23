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
#include "VideoFilterYUV422toRGB.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(FilterYUV422toRGB);

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

