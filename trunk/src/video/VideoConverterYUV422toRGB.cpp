// -*- c++ -*- ///////////////////////////////////////////////////////////////
/*! \file VideoConverterYUV422toRGB.cpp
 *  \brief Image conversion routines YUV->RGB
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
 * Revision 1.2  2003/01/10 13:46:46  muehlenf
 * Some corrections and tests, first working version.
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#include "VideoConverterYUV422toRGB.h"

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
    VideoConverterYUV422toRGB::VideoConverterYUV422toRGB()
    {
    }

//---------------------------------------------------------------
    VideoConverterYUV422toRGB::~VideoConverterYUV422toRGB()
    {
    }


//---------------------------------------------------------------
    void VideoConverterYUV422toRGB::operator() (const unsigned char * const src_img,
                                                unsigned char * tgt_img,
	        			        int num_pixels) const
    {
	for (int i = 0; i < 2*num_pixels; i += 4)
	{
	    register int u  = src_img[i];
	    register int y0 = src_img[i+1];
	    register int v  = src_img[i+2];
	    register int y1 = src_img[i+3];

	    *(tgt_img++) = t_r[(y0<<8)|v];
	    *(tgt_img++) = t_g2[(y0<<8)|t_g1[(u<<8)|v]];
	    *(tgt_img++) = t_b[(y0<<8)|u];
	    
	    *(tgt_img++) = t_r[(y1<<8)|v];
	    *(tgt_img++) = t_g2[(y1<<8)|t_g1[(u<<8)|v]];
	    *(tgt_img++) = t_b[(y1<<8)|u];
	}
    }

};

