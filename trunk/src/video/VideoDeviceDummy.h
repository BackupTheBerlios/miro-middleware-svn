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
 * Revision 1.4  2003/06/03 10:25:32  hutz
 * complete revamp of the video service
 * the interface changes slightly to allow for better access
 * removed unnecessary copies
 * added a complete filter framework for server side image processing
 * added a library miroVideo.a for easy customization to own video service
 * derivates
 * the dummy video device now displays an image
 *
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

  class DeviceDummy : public Device
  {
    typedef Device Super;

  public:
    DeviceDummy(const Miro::ImageFormatIDL& _inputFormatParameters);
    virtual ~DeviceDummy();
	    	    
    FILTER_PARAMETERS_FACTORY(DeviceDummy);

    virtual void init(FilterParameters const * _params);
    virtual void fini();
    virtual void acquireOutputBuffer();
    virtual void releaseOutputBuffer();

  private:
    //! connection simulation flag
    bool is_connected_;
  };
};

#endif // VIDEODEVICEDUMMY_H

