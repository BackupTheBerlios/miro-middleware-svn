/* -*- c++ -*- */
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
 * Revision 1.3  2003/05/13 21:58:49  hutz
 * removing the bridge pattern between VideoDevice and VideoDeviceBase
 * making VideoDevice* a direct descendant of VideoDevice
 *
 * Revision 1.2  2003/05/13 20:50:21  hutz
 * cleaning up the video service, getting rid of VideoConnection
 *
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

#include "VideoDevice.h"

namespace Video
{
  /** Dummy video device for testing the interface
   */

  class VideoDeviceDummy : public VideoDevice
  {
    typedef VideoDevice Super;

  public:
    VideoDeviceDummy(Parameters const * _params = Parameters::instance());
    virtual ~VideoDeviceDummy();
	    	    
    virtual void connect();
    virtual void disconnect();
		
    virtual void * grabImage(ACE_Time_Value & _timeStamp) const;

  private:
    //! connection simulation flag
    bool is_connected_;
    //! indirect framebuffer
    char * buffer_;
  };
};

#endif // VIDEODEVICEDUMMY_H

