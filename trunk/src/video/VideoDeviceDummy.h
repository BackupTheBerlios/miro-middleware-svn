/*! \file VideoDeviceDummy.h
 *  \brief Dummy implementation for a video device (interface)
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
 * Revision 1.2  2003/03/10 07:40:34  muehlenf
 * Additional parameters for firewire cameras.
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#ifndef VIDEODEVICEDUMMY_H
#define VIDEODEVICEDUMMY_H

#include "VideoDeviceBase.h"

namespace Video
{
    /** Dummy video device for testing the interface
     */

    class VideoDeviceDummy : public VideoDeviceBase
    {
    public:
        VideoDeviceDummy();
        virtual ~VideoDeviceDummy();
	    	    
	virtual	void*	grabImage(ACE_Time_Value & _timeStamp) const;

	virtual	void	handleConnect(const int fd, const Parameters& params);
	virtual	void	handleDisconnect();
		
    private:
        //! connection simulation flag
        bool is_connected_;
	//! indirect framebuffer
        char * buffer_;
    };
};

#endif // VIDEODEVICEDUMMY_H

