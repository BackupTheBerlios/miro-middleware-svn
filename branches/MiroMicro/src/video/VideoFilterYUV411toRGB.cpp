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
#include "VideoFilterYUV411toRGB.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

#include <iostream>
#include <algorithm>

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(FilterYUV411toRGB);

  //---------------------------------------------------------------
  FilterYUV411toRGB::FilterYUV411toRGB(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    if (_inputFormat.palette != Miro::YUV_411)
      throw Miro::Exception("Incompatible input format for FilterYUV411toRGB.");
    outputFormat_.palette = Miro::RGB_24;
  }

  //---------------------------------------------------------------
  FilterYUV411toRGB::~FilterYUV411toRGB()
  {
  }

  //---------------------------------------------------------------
  void 
  FilterYUV411toRGB::process()
  {
    unsigned char const * src_img = inputBuffer();
    unsigned char * tgt_img = outputBuffer();
    unsigned int srcImgSize = getImageSize(inputFormat_);

    for (unsigned int i = 0; i < srcImgSize; i += 6) {
      register int u  = src_img[i];
      register int y0 = src_img[i+1];
      register int y1 = src_img[i+2];
      register int v  = src_img[i+3];
      register int y2 = src_img[i+4];
      register int y3 = src_img[i+5];
	    
      *(tgt_img++) = t_r[(y0<<8)|v];
      *(tgt_img++) = t_g2[(y0<<8)|t_g1[(u<<8)|v]];
      *(tgt_img++) = t_b[(y0<<8)|u];
	    
      *(tgt_img++) = t_r[(y1<<8)|v];
      *(tgt_img++) = t_g2[(y1<<8)|t_g1[(u<<8)|v]];
      *(tgt_img++) = t_b[(y1<<8)|u];
	    
      *(tgt_img++) = t_r[(y2<<8)|v];
      *(tgt_img++) = t_g2[(y2<<8)|t_g1[(u<<8)|v]];
      *(tgt_img++) = t_b[(y2<<8)|u];
	    
      *(tgt_img++) = t_r[(y3<<8)|v];
      *(tgt_img++) = t_g2[(y3<<8)|t_g1[(u<<8)|v]];
      *(tgt_img++) = t_b[(y3<<8)|u];
    }
  }  
};
