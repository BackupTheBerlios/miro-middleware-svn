// -*- c++ -*- ///////////////////////////////////////////////////////////////
/*! \file VideoConverterYUV422toYUV.cpp
 *  \brief Image conversion routines YUV422->YUV
 *
 * -----------------------------------------------------
 *
 * RoboCup Middle Size Team, Technische Universitaet Graz
 * "Mostly Harmless" 
 * http://www.robocup.tugraz.at
 *
 * Project: Miro::Video
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 * $Log$
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#include "VideoConverterYUV422toYUV.h"

#include <miro/Exception.h>

#include <iostream>
#include <algorithm>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
//---------------------------------------------------------------
    VideoConverterYUV422toYUV::VideoConverterYUV422toYUV()
    {
    }

//---------------------------------------------------------------
    VideoConverterYUV422toYUV::~VideoConverterYUV422toYUV()
    {
    }


//---------------------------------------------------------------
    void VideoConverterYUV422toYUV::operator() (const unsigned char * const src_img,
                                                unsigned char * tgt_img,
				                int num_pixels) const
    {
	for (int i = 0; i < 2*num_pixels; i += 4)
	{
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
