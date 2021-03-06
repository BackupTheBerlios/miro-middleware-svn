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
#include "VideoFilterYUV420PtoRGB.h"

#include "miro/VideoHelper.h"
#include "miro/Exception.h"

#include <iostream>
#include <algorithm>

namespace Video
{
  long int FilterYUV420PtoRGB::crv_tab[256];
  long int FilterYUV420PtoRGB::cbu_tab[256];
  long int FilterYUV420PtoRGB::cgu_tab[256];
  long int FilterYUV420PtoRGB::cgv_tab[256];
  long int FilterYUV420PtoRGB::tab_76309[256];
  unsigned char FilterYUV420PtoRGB::clp[1024];



  //---------------------------------------------------------------
  FilterYUV420PtoRGB::FilterYUV420PtoRGB(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    if (_inputFormat.palette != Miro::YUV_420P)
      throw Miro::Exception("Incompatible input format for FilterYUV420PtoRGB.");
    outputFormat_.palette = Miro::RGB_24;

    InitConvtTbl();

  }

  //---------------------------------------------------------------
  FilterYUV420PtoRGB::~FilterYUV420PtoRGB()
  {
  }

  //---------------------------------------------------------------
  void
  FilterYUV420PtoRGB::process()
  {
    unsigned char const * src_img = inputBuffer();
    unsigned char * tgt_img = outputBuffer();
    //unsigned int srcImgSize = getImageSize(inputFormat_);
    int width = inputFormat_.width;
    int height = inputFormat_.height;
    unsigned char const *src0;
	unsigned char const *src1;
	unsigned char const *src2;
	int y1,y2,u,v;
	unsigned char const *py1,*py2;
	int i,j, c1, c2, c3, c4;
	unsigned char *d1, *d2;

	//Initialization
	src0=src_img;
	src1=src_img+width*height;
	src2=src_img+width*height+width*height/4;

	py1=src0;
	py2=py1+width;
	d1=tgt_img;
	d2=d1+3*width;
 	for (j = 0; j < height; j += 2) {
		for (i = 0; i < width; i += 2) {

			u = *src1++;
			v = *src2++;

			c1 = crv_tab[v];
			c2 = cgu_tab[u];
			c3 = cgv_tab[v];
			c4 = cbu_tab[u];

			//up-left
            y1 = tab_76309[*py1++];
			*d1++ = clp[384+((y1 + c1)>>16)];
			*d1++ = clp[384+((y1 - c2 - c3)>>16)];
            *d1++ = clp[384+((y1 + c4)>>16)];

			//down-left
			y2 = tab_76309[*py2++];
			*d2++ = clp[384+((y2 + c1)>>16)];
			*d2++ = clp[384+((y2 - c2 - c3)>>16)];
            *d2++ = clp[384+((y2 + c4)>>16)];

			//up-right
			y1 = tab_76309[*py1++];
			*d1++ = clp[384+((y1 + c1)>>16)];
			*d1++ = clp[384+((y1 - c2 - c3)>>16)];
			*d1++ = clp[384+((y1 + c4)>>16)];

			//down-right
			y2 = tab_76309[*py2++];
			*d2++ = clp[384+((y2 + c1)>>16)];
			*d2++ = clp[384+((y2 - c2 - c3)>>16)];
            *d2++ = clp[384+((y2 + c4)>>16)];
		}
		d1 += 3*width;
		d2 += 3*width;
		py1+=   width;
		py2+=   width;
	}

  }



  void FilterYUV420PtoRGB::InitConvtTbl()
  {
   long int crv,cbu,cgu,cgv;
   int i,ind;

   crv = 104597; cbu = 132201;  /* fra matrise i global.h */
   cgu = 25675;  cgv = 53279;

   for (i = 0; i < 256; i++) {
      crv_tab[i] = (i-128) * crv;
      cbu_tab[i] = (i-128) * cbu;
      cgu_tab[i] = (i-128) * cgu;
      cgv_tab[i] = (i-128) * cgv;
      tab_76309[i] = 76309*(i-16);
   }

   for (i=0; i<384; i++)
	  clp[i] =0;
   ind=384;
   for (i=0;i<256; i++)
	   clp[ind++]=i;
   ind=640;
   for (i=0;i<384;i++)
	   clp[ind++]=255;
   }

};
