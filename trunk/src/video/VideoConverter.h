/*! \file VideoConverter.h
 *  \brief Image conversion routines base (interface)
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

#ifndef VIDEOCONVERTER_H
#define VIDEOCONVERTER_H

namespace Video
{
    //! conversion base
    class VideoConverter
    {
    public:
        VideoConverter();
	virtual ~VideoConverter();
	
	virtual void operator() (const unsigned char * const src_img, unsigned char * tgt_img, int num_pixels) const = 0;
    };
};

#endif // VIDEOCONVERTER_H

