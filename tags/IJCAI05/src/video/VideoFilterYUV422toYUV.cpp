// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

/*! \file VideoFilterYUV422toYUV.cpp
 *  \brief Image conversion routines YUV422->YUV
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilterYUV422toYUV.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(FilterYUV422toYUV);

  //---------------------------------------------------------------
  FilterYUV422toYUV::FilterYUV422toYUV(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    if (_inputFormat.palette != Miro::YUV_422)
      throw Miro::Exception("Incompatible input format for FilterYUV422toYUV.");
    outputFormat_.palette = Miro::YUV_24;
  }
  
  //---------------------------------------------------------------
  FilterYUV422toYUV::~FilterYUV422toYUV()
  {
  }

  //---------------------------------------------------------------
  void
  FilterYUV422toYUV::process()
  {
    unsigned char const * src_img = inputBuffer();
    unsigned char * tgt_img = outputBuffer();
    unsigned int srcImgSize = getImageSize(inputFormat_);

    for (unsigned int i = 0; i < srcImgSize; i += 4) {
      register int u  = src_img[i];
      register int y0 = src_img[i+1];
      register int v  = src_img[i+2];
      register int y1 = src_img[i+3];
      
      *(tgt_img++) = y0;
      *(tgt_img++) = u;
      *(tgt_img++) = v;

      *(tgt_img++) = y1;
      *(tgt_img++) = u;
      *(tgt_img++) = v;
    }
  }
};
