// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

/*! \file FilterOmni2Pan.cpp
 *  \brief Image conversion routines from omnicam to panoramic view
 *  \author Roland Reichle
 */

#include "VideoFilterOmni2Pan.h"
#include "miro/Log.h"
#include "miro/VideoHelper.h"
#include "miro/Exception.h"

#include <algorithm>
#include <cmath>

namespace Video
{
  double FilterOmni2Pan::cosLookup_[360];
  double FilterOmni2Pan::sinLookup_[360];

  //---------------------------------------------------------------
  FilterOmni2Pan::FilterOmni2Pan(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    MIRO_LOG_CTOR("Video::FilterOmni2Pan");
    if (_inputFormat.palette != Miro::RGB_24)
      throw Miro::Exception("Incompatible input format for FilterOmni2Pan.");
    outputFormat_.palette = Miro::RGB_24;
    outputFormat_.width = IMAGE_WIDTH;
    outputFormat_.height = IMAGE_HEIGHT;

    middleX_ = inputFormat_.width / 2;
    middleY_ = inputFormat_.height / 2;

    buildLookupTables();
  }

  //---------------------------------------------------------------
  FilterOmni2Pan::~FilterOmni2Pan()
  {
  }

  //---------------------------------------------------------------
  void
  FilterOmni2Pan::process()
  {
    unsigned char const * srcImg = inputBuffer();
    unsigned char * tgtImg = outputBuffer();
    unsigned char * tgtImgEnd = tgtImg + IMAGE_WIDTH * IMAGE_HEIGHT * 3;
    int * srcOffset = srcOffset_;

    for (; tgtImg != tgtImgEnd; ++tgtImg, ++srcOffset) {
      *tgtImg = *(srcImg + *srcOffset);
    }
  }

  void
  FilterOmni2Pan::buildLookupTables()
  {
    for (unsigned int i = 0; i < 360; ++i) {
      cosLookup_[i] = cos(M_PI * i / 180.0);
      sinLookup_[i] = sin(M_PI * i / 180.0);
    }

    int * srcOffset = srcOffset_;
    for (unsigned int dist = 0; dist < outputFormat_.height; dist++) {
      for (unsigned int angle = 0; angle < outputFormat_.width; angle++) {
	for(int color = 0; color < 3; color++) {
	  *(srcOffset++) = 
	    (middleX_ + (int)floor(dist * cosLookup_[angle])) * 3 +
	    (middleY_ + (int)floor(dist * sinLookup_[angle])) * 3 * inputFormat_.width + 
	    color;
	}
      }
    }
  }
}
