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

using std::endl;
using std::cout;

namespace Video
{
  double FilterOmni2Pan::cosLookup_[360];
  double FilterOmni2Pan::sinLookup_[360];

  FILTER_PARAMETERS_FACTORY_IMPL(FilterOmni2Pan);

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

    for (; tgtImg != tgtImgEnd; ++srcOffset) {
      *tgtImg++ = *(srcImg + *srcOffset);
      *tgtImg++ = *(srcImg + *srcOffset + 1);
      *tgtImg++ = *(srcImg + *srcOffset + 2);
    }
  }

  void
  FilterOmni2Pan::buildLookupTables()
  {
    for (unsigned int i = 0; i < 360; ++i) {
      cosLookup_[i] = cos(M_PI * i / 180.0);
      sinLookup_[i] = sin(M_PI * i / 180.0);
    }

    double scaleFactor = (double)radius_/(double)outputFormat_.height;

    int * srcOffset = srcOffset_;
    for (unsigned int dist = 0; dist < outputFormat_.height; dist++) {
      for (unsigned int angle = 0; angle < outputFormat_.width; angle++) {
	*(srcOffset++) = 
	  std::min((int)inputFormat_.width-1,
		   std::max(0,
			    (middleX_ + (int)floor((double)dist*scaleFactor*cosLookup_[angle])))) * 3 +
	  std::min((int)inputFormat_.height-1,
		   std::max(0,
			    (middleY_ + (int)floor((double)dist*scaleFactor*sinLookup_[angle])))) * 3 * inputFormat_.width;
      }
    }
  }


  void FilterOmni2Pan::init(Miro::Server& _server, Video::FilterParameters const * _params)
  {
    Super::init(_server, _params);

    FilterOmni2PanParameters const * params = 
      dynamic_cast<FilterOmni2PanParameters const *>(_params);
    assert(params != NULL);

    middleX_ = params->centerX;
    middleY_ = params->centerY;
    radius_ = params->radius;

    buildLookupTables();
  }
}
