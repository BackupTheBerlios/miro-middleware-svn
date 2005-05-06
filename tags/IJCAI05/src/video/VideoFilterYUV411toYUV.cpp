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

/*! \file VideoFilterYUV411toYUV.cpp
 *  \brief Image conversion routines YUV411->YUV
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilterYUV411toYUV.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

namespace Video
{
  //---------------------------------------------------------------
  FilterYUV411toYUV::FilterYUV411toYUV(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    if (_inputFormat.palette != Miro::YUV_411)
      throw Miro::Exception("Incompatible input format for FilterYUV411toYUV.");
    outputFormat_.palette = Miro::YUV_24;
  }

  //---------------------------------------------------------------
  FilterYUV411toYUV::~FilterYUV411toYUV()
  {
  }


  //---------------------------------------------------------------
  void 
  FilterYUV411toYUV::process()
  {
    unsigned char const * src_img = inputBuffer();
    unsigned char * tgt_img = outputBuffer();
    int srcImgSize = getImageSize(inputFormat_);

    for (int i = 0; i < srcImgSize; i += 6) {
      register int u  = src_img[i];
      register int y0 = src_img[i+1];
      register int y1 = src_img[i+2];
      register int v  = src_img[i+3];
      register int y2 = src_img[i+4];
      register int y3 = src_img[i+5];
      
      *(tgt_img++) = y0;
      *(tgt_img++) = u;
      *(tgt_img++) = v;
      
      *(tgt_img++) = y1;
      *(tgt_img++) = u;
      *(tgt_img++) = v;
      
      *(tgt_img++) = y2;
      *(tgt_img++) = u;
      *(tgt_img++) = v;
	
      *(tgt_img++) = y3;
      *(tgt_img++) = u;
      *(tgt_img++) = v;
    }
  }
};
