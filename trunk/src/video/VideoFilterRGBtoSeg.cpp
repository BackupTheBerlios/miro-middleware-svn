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
    
    int rest = inputFormat_.height % 32;
    if(rest != 0){
       tmp_p = bitImage_;
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

    int ptroff = (rest != 0)?1:0;
    
    int numInts = inputFormat_.height/32;
    for(int y = 0; y < numInts; y++){
       tmp_p = bitImage_ + (y+ptroff)*inputFormat_.width;
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
    
    numInts += ptroff;
    unsigned int first, pre, actual;
    for(int i = 0; i < numInts; i++){
       int_p = bitImage_ + i*inputFormat_.width;
       pre = *int_p;
       first = *int_p;
       *int_p = *int_p | *(int_p + inputFormat_.widht - 1) | *(int_p + 1);
       int_p++; 
       for(int j = 1; j < inputFormat_.width - 1; j++){
          actual = *int_p; 
          *int_p = pre | actual | *(int_p + 1);
	  pre = actual;
	  int_p++;
       
       }
       *int_p = pre | *int_p | first;
    }
    
    
    int index;
    int tmpInd;
    int z = 0;
    
    for(int i = 0; i < inputFormat_.width; i++){
       int_p = bitImage_ + (numInts -1)*inputFormat_.width;
       z = 0;
       index = 0;
       tmpInd = 0;
       while(*int_p == 0 && z < numInts){
          int_p -= inputFormat_.width;
	  z++;
	  tmpInd += 32;
       
       }
       if(z == numInts){
          distanceProfile_[i] = 5000;
       }
       else{
          for(int j = z; j < numInts; k++){
	     if(*int_p != 0){
	        actual = *int_p;
		int range = (j == numInts - 1)?rest-2:30;
		for(int k = 0; k < range; k++){
		   if(actual & 0x00000007 == 0x000000007)
		      index = tmpInd;
		      break;
		   }   
                   actual = actual >> 1;
		   tmpInd++;		
		}
		if((index == 0) &&(j < numInts - 1) && 
		   (actual & 0x00000003 == 0x00000003) &&
		   (*(int_p - inputFormat_.width) & 0x00000001 = 0x00000001))
		      index = tmpInd;
		else if(index == 0){
		   actual = actual >> 1;
		   tmpInd++;
		   if((j < numInts - 1) &&
		      (actual & 0x00000001 == 0x00000001) &&
		      (*(int_p - inputFormat_.width) & 0x00000003 = 0x00000003))
		         index = tmpInd;
	           }
		
		} 
	     }    
	     else{
	        int_p -= inputFormat_.width;      
	        tmpInd +=32;
	     }
	     
	     if(index != 0)
	        break;   
             
	  }
                    
          
                
       }
       if(index == 0)
          distanceProfile_[i] = 5000;
       else
          distanceProfile_[i] = distLookup_[index];
    
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
