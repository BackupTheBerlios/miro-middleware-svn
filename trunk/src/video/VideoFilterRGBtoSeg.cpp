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
    
    buildLookupTables();
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

    unsigned int length = inputFormat_.width * inputFormat_.height;

    for(unsigned int x = 0; x < length; ++x){

          *(tgt_img++) =  (r_lookup[*(src_img++)] +  g_lookup[*(src_img++)] + b_lookup[*(src_img++)] < 80.0)?0:255;

    }
    
    /*unsigned int * int_p = bitImage_;
    unsigned int * tmp_p;
    unsigned int mask;
    
    unsigned int zaehler = 0;
    
    int numInts = inputFormat_.height/32;
    for(int y = 0; y < numInts; y++){
       tmp_p = bitImage_ + y*inputFormat_.width;
       for(int w = 0; w < 32; w++){
          int_p = tmp_p;
          for(unsigned int z = 0; z < inputFormat_.width; z++){
          	  *int_p << 1;
		  mask =  (r_lookup[*(src_img++)] +  g_lookup[*(src_img++)] + b_lookup[*(src_img++)] < 80.0)?0:0x00000001;
	          *int_p |= mask;
	          int_p++;
		  zaehler++;
	  }
       }
    }
    int rest = inputFormat_.height % 32;
    if(rest != 0){
       tmp_p = bitImage_ + numInts*inputFormat_.width;
       for(int w = 0; w < rest; w++){
          int_p = tmp_p;
          for(int z = 0; z < inputFormat_.width; z++){
          	  *int_p << 1;
		  mask =  (r_lookup[*(src_img++)] +  g_lookup[*(src_img++)] + b_lookup[*(src_img++)] < 80.0)?0:0x00000001;
	          *int_p |= mask;
	          int_p++;
		  zaehler++;
	  }
       }
    }
    
    //std::cout << "Debug output: RGBtoSeg Counter: " << zaehler << std::endl;
        
*/

  }
  
  void FilterRGBtoSeg::buildLookupTables()
  {
  
     for(int i = 0; i < 256; i++){
        r_lookup[i] = (float)i * 0.299;
	g_lookup[i] = (float)i * 0.587;
	b_lookup[i] = (float)i * 0.114;
     }  
  
  
  }

};
