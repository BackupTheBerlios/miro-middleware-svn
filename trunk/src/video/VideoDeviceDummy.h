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
 * Revision 1.6  2003/10/27 13:19:21  hutz
 * making DummyDevice capable of playing image series
 *
 * Revision 1.5  2003/10/17 13:31:42  hutz
 * big video service update
 * we now support filters with multiple input buffers
 * we also support the first version of a video broker interface for
 * synchronised image access and filter tree monitoring
 * - it is not yet implementation complete...
 * we now release buffers for reading as soon as all processing is done
 * we now free buffers as soon as all successors are done
 * added buffer manager for the individual devices
 * connection management is now thread safe (at least has one severe bug less)
 * TODO: documentation update
 * TODO: video broker impl
 * TODO: multiple devices
 *
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

#ifndef VideoDeviceDummy_h
#define VideoDeviceDummy_h

#include "VideoDevice.h"

#include <vector>
#include <string>

namespace Video
{
  /// Dummy video device that reads images from disk.
  /**
   * Image or a directory to look for images can be specifed.
   */
  class DeviceDummy : public Device
  {
    typedef Device Super;

  public:
    DeviceDummy(const Miro::ImageFormatIDL& _inputFormatParameters);
    virtual ~DeviceDummy();
	    	    
    FILTER_PARAMETERS_FACTORY(DeviceDummy);

  protected:
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void process();

    //--------------------------------------------------------------------------
    // protected data

    DeviceDummyParameters const * params_;

    unsigned int counter_;
    std::vector<std::string> files_;
  };
}

#endif // VideoDeviceDummy_h
