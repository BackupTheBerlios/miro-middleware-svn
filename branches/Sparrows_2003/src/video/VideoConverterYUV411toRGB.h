// -*- c++ -*- ///////////////////////////////////////////////////////////////
/*! \file VideoConverterYUV411toRGB.h
 *  \brief Image conversion routines YUV411->RGB (interface)
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

#ifndef VIDEOCONVERTERYUV411TORGB_H
#define VIDEOCONVERTERYUV411TORGB_H

#include "VideoConverterYUVbase.h"

namespace Video
{
    //! Lookup table driven conversion from YUV411 to RGB
    class VideoConverterYUV411toRGB : public VideoConverterYUVbase
    {
    public:
        VideoConverterYUV411toRGB();
	virtual ~VideoConverterYUV411toRGB();
	
	virtual void operator() (const unsigned char * const src_img, unsigned char * tgt_img, int num_pixels) const;
    };
};

#endif // VIDEOCONVERTERYUV422_H

