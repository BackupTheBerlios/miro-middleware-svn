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

/*! \file FilterRGBtoSeg.cpp
 *  \brief Image conversion routines YUV->RGB
 *  \author Roland Reichle 
 */

#include "VideoFilterRGBtoSeg.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"
#include "miro/Log.h"
#include <iostream>
#include <algorithm>
#include <cmath>

namespace Video
{


  //---------------------------------------------------------------
  FilterRGBtoSeg::FilterRGBtoSeg(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    MIRO_LOG_CTOR("Video::FilterRGBtoSeg");
    if (_inputFormat.palette != Miro::RGB_24)
      throw Miro::Exception("Incompatible input format for FilterRGBtoSeg.");
    outputFormat_.palette = Miro::GREY_8;

  }

  //---------------------------------------------------------------
  FilterRGBtoSeg::~FilterRGBtoSeg()
  {
  }

  //---------------------------------------------------------------
  void
  FilterRGBtoSeg::process()
  {
    unsigned char const * src_img = inputBuffer();
    unsigned char * tgt_img = outputBuffer();



    for(unsigned int x = 0; x < inputFormat_.width; x++){
       for(unsigned int y = 0; y < inputFormat_.height; y++){

          *(tgt_img++) =  ((unsigned char) (*(src_img++)*0.299 +  *(src_img++)*0.587 + *(src_img++)*0.114) < 80)?0:255;

       }

    }



  }




};
