/*! \file VideoConverterYUVbase.cpp
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
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#include "VideoConverterYUVbase.h"

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
    VideoConverterYUVbase::VideoConverterYUVbase()
    {
        initTables();
    }

//---------------------------------------------------------------
    VideoConverterYUVbase::~VideoConverterYUVbase()
    {
    }


//---------------------------------------------------------------
    void VideoConverterYUVbase::initTables()
    {
        for (int i = 0; i < 256; i++)
	    for (int j = 0; j < 256; j++)
	    {
	        int r = i + (((j-128)*1434)/2048);
		int b = i + (((j-128)*2078)/2048);
		int g1 = (((i-128)*406)/2048)+(((j-128)*595)/2048);
		int g2 = i-j;
		
		t_r[(i<<8)|j] = min(max(0,r),255);
		t_b[(i<<8)|j] = min(max(0,b),255);
		t_g1[(i<<8)|j] = min(max(0,g1),255);
		t_g2[(i<<8)|j] = min(max(0,g2),255);
	    }
    }
};
