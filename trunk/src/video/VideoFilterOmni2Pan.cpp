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

/*! \file FilterOmni2Pan.cpp
 *  \brief Image conversion routines YUV->RGB
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilterOmni2Pan.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

#include <iostream>
#include <algorithm>
#include <cmath>

namespace Video
{

  double FilterOmni2Pan::cos_lookup[360];
  double FilterOmni2Pan::sin_lookup[360];


  //---------------------------------------------------------------
  FilterOmni2Pan::FilterOmni2Pan(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    if (_inputFormat.palette != Miro::RGB_24)
      throw Miro::Exception("Incompatible input format for FilterOmni2Pan.");
    outputFormat_.palette = Miro::RGB_24;
    outputFormat_.width = 360;
    outputFormat_.height = 120;

    middle_x = inputFormat_.width / 2;
    middle_y = inputFormat_.height / 2;
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
    unsigned char const * src_img = inputBuffer();
    unsigned char * tgt_img = outputBuffer();


    for(unsigned int dist = 0; dist < outputFormat_.height - 1; dist++) {
        for(unsigned int angle = 0; angle < outputFormat_.width; angle++) {
       	    for(int color = 0; color < 3; color++) {
		*(tgt_img++) = *(src_img + (middle_x + (int)floor(dist * cos_lookup[angle]))*3 +
		                         (middle_y + (int)floor(dist * sin_lookup[angle]))*3*inputFormat_.width + color);

	    }
	}
    }






  }

  void FilterOmni2Pan::buildLookupTables(){

      for(int i = 0; i < 360; i++){
         cos_lookup[i] = cos(M_PI * i / 180.0);
	 sin_lookup[i] = sin(M_PI * i / 180.0);

      }

  }



};
