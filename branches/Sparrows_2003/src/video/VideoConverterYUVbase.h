// -*- c++ -*- ///////////////////////////////////////////////////////////////
/*! \file VideoConverterYUVbase.h
 *  \brief Image conversion routines YUV->RGB (interface)
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

#ifndef VIDEOCONVERTERYUVBASE_H
#define VIDEOCONVERTERYUVBASE_H

#include "VideoConverter.h"

namespace Video
{
    //! Lookup table driven conversion from YUV422 to RGB
    class VideoConverterYUVbase : public VideoConverter
    {
    public:
        VideoConverterYUVbase();
	virtual ~VideoConverterYUVbase();
	
    protected:
        void initTables();
			
        unsigned char t_r[65536];
	unsigned char t_b[65536];
	unsigned char t_g1[65536];
	unsigned char t_g2[65536];
    };
};

#endif // VIDEOCONVERTERYUVbase_H

