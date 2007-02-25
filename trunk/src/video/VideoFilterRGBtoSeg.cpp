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
#include "VideoFilterRGBtoSeg.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"
#include "miro/Log.h"
#include "miro/Server.h"

#include <iostream>
#include <algorithm>
#include <cmath>

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(FilterRGBtoSeg);

  //---------------------------------------------------------------
  FilterRGBtoSeg::FilterRGBtoSeg(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat),
    params_(NULL)
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
    float threshold = (params_ != NULL)? (params_->threshold) : 80.0;

    int dbg_counter = 0;
    
    std::cout << "VideoFilterRGBtoSeg threshold: " << threshold << std::endl;

    
    for (unsigned int x = 0; x < length; ++x) {
      float sum = r_lookup[*(src_img++)];
      sum += g_lookup[*(src_img++)];
      sum += b_lookup[*(src_img++)];

      *(tgt_img++) =  (unsigned int) rintf(sum);
    }
    
    bzero(bitImage_, 360 * 4 * 4);
    unsigned int * int_p = bitImage_;
    unsigned int * tmp_p;
    unsigned int mask;
    
    unsigned int zaehler = 0;
    
    src_img = inputBuffer();
    tgt_img = outputBuffer();
    unsigned int rest = inputFormat_.height % 32;
    if(rest != 0){
       tmp_p = bitImage_;
       for(unsigned int w = 0; w < rest; w++){
          int_p = tmp_p;
          for(unsigned int z = 0; z < inputFormat_.width; z++){
            *int_p = (*int_p << 1);

	    float sum = r_lookup[*(src_img++)];
	    sum += g_lookup[*(src_img++)];
	    sum += b_lookup[*(src_img++)];

	    mask =  (sum < threshold)? 1 : 0;
	    
	    if(mask)
		*(tgt_img) = 127;
	    tgt_img++;
            *int_p |= mask;
            int_p++;
   	    zaehler++;
	  }
       }
    }

    int ptroff = (rest != 0)?1:0;
    
    int numInts = inputFormat_.height/32;
    for(int y = 0; y < numInts; y++){
       tmp_p = bitImage_ + (y+ptroff)*inputFormat_.width;
       for(int w = 0; w < 32; w++){
          int_p = tmp_p;
          for(unsigned int z = 0; z < inputFormat_.width; z++){
             *int_p = (*int_p << 1);

	     float sum = r_lookup[*(src_img++)];
	     sum += g_lookup[*(src_img++)];
	     sum += b_lookup[*(src_img++)];
	     
	     mask =  (sum < threshold)? 1 : 0;
	     
	     if(mask)
	       *(tgt_img) = 127;
	     tgt_img++;
            *int_p |= mask;
            int_p++;
            zaehler++;
	  }
       }
    }
   
    numInts += ptroff;
    unsigned int first, pre, actual;
    for(int i = 0; i < numInts; i++){
       int_p = bitImage_ + i*inputFormat_.width;
       pre = *int_p;
       first = *int_p;
       *int_p = *int_p & *(int_p + inputFormat_.width - 1) & *(int_p + 1);
       int_p++; 
       for(unsigned int j = 1; j < inputFormat_.width - 1; j++){
          actual = *int_p; 
          *int_p = pre & actual & *(int_p + 1);
	  pre = actual;
	  int_p++;
       
       }
       *int_p = pre & *int_p & first;
    }
    
    
    int index;
    int tmpInd;
    int z = 0;

    tgt_img = outputBuffer();
    
    for(unsigned int i = 0; i < inputFormat_.width; i++){
       int_p = bitImage_ + (numInts -1)*inputFormat_.width + i;
       z = 0;
       index = -1;
       tmpInd = 0;
       while(*int_p == 0 && z < numInts) {
          int_p -= inputFormat_.width;
	  z++;
	  tmpInd += 32;
       }
       if(z == numInts) {
          distanceProfile_[i] = 5000;
       }
       else {
	  dbg_counter++;

          for(int j = z; j < numInts; j++) {
	     if(*int_p != 0) {
	        actual = *int_p;
		int range = (j == numInts - 1)?rest-2:30;
		for (int k = 0; k < range; k++) {
		   if(actual & 0x00000007 == 0x00000007) {
		      index = tmpInd;
		      break;
		   }   
                   actual = actual >> 1;
		   tmpInd++;		
		}
		if((index == -1) &&(j < numInts - 1) && 
		   (actual & 0x00000003 == 0x00000003) &&
		   (*(int_p - inputFormat_.width) & 0x00000001 == 0x00000001)) {
		      index = tmpInd;
		}
		else if(index == -1) {
		   actual = actual >> 1;
		   tmpInd++;
		   if((j < numInts - 1) &&
		      (actual & 0x00000001 == 0x00000001) &&
		      (*(int_p - inputFormat_.width) & 0x00000003 == 0x00000003)) {
		         index = tmpInd;
		   }
	           
		
		} 
	     }    
	     else{
	        int_p -= inputFormat_.width;      
	        tmpInd +=32;
	     }
	     
	     
	     if(index != -1)
	        break;   
             
	  }
                    
          
                
       }
       if(index == -1)
       	 distanceProfile_[i] = 5000;
       else {
       	 distanceProfile_[i] = distLookup_[index];
	 for(unsigned int m = 0; m < inputFormat_.height - index; m++) {
	         *(tgt_img + (m * inputFormat_.width) + i) = 255; 
	 }
       }
    
    }
    
    std::cout << "RGBtoSeg Debug Counter: " << dbg_counter << std::endl;
    
    //std::cout << "Debug output: RGBtoSeg Counter: " << zaehler << std::endl;

  }
  
  void FilterRGBtoSeg::buildLookupTables()
  {
  
     for(int i = 0; i < 256; i++){
        r_lookup[i] = (float)i * 0.299;
	g_lookup[i] = (float)i * 0.587;
	b_lookup[i] = (float)i * 0.114;
     }  
  
  
  }

  void FilterRGBtoSeg::init(Miro::Server& _server, FilterParameters const * _params){

   params_ = dynamic_cast<FilterRGBtoSegParameters const *> (_params);
   Super::init(_server, _params);

}

  
  
  
};



