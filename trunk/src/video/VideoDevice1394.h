/* -*- c++ -*- */
/*! \file VideoDevice1394.h
 *  \brief Implementation for a firewire video device (interface)
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
 * Revision 1.8  2004/02/09 17:28:56  graz
 * Added control-interface to VideoDevice1394 & Dialog.
 *
 * Revision 1.7  2003/10/17 13:31:42  hutz
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
 * Revision 1.6  2003/06/03 11:03:36  hutz
 * fixed copy paste error in macros.miro.GNU
 * removed the HAVE_* macros in VideoDevice1394.*
 *
 * Revision 1.5  2003/06/03 10:25:32  hutz
 * complete revamp of the video service
 * the interface changes slightly to allow for better access
 * removed unnecessary copies
 * added a complete filter framework for server side image processing
 * added a library miroVideo.a for easy customization to own video service
 * derivates
 * the dummy video device now displays an image
 *
 * Revision 1.4  2003/05/13 21:58:49  hutz
 * removing the bridge pattern between VideoDevice and VideoDeviceBase
 * making VideoDevice* a direct descendant of VideoDevice
 *
 * Revision 1.3  2003/05/13 20:50:20  hutz
 * cleaning up the video service, getting rid of VideoConnection
 *
 * Revision 1.2  2003/05/12 16:34:35  hutz
 * bug hunt on fire wire
 *
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.4  2003/03/11 18:19:16  muehlenf
 * Added device-parameter.
 *
 * Revision 1.3  2003/03/10 07:40:34  muehlenf
 * Additional parameters for firewire cameras.
 *
 * Revision 1.2  2003/01/14 10:19:53  muehlenf
 * Started using configure & automake.
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#ifndef VIDEODEVICE1394_H
#define VIDEODEVICE1394_H

#include "VideoDevice.h"
#include "idl/VideoControlC.h"

#include <libraw1394/raw1394.h>
#include <libdc1394/dc1394_control.h>

namespace Video
{
  class ControlImpl;

  //! Firewire camera support.
  class Device1394 : public Device
  {
    typedef Device Super;
    
  public:
    Device1394(const Miro::ImageFormatIDL& _inputFormat);
    virtual ~Device1394();
	
    FILTER_PARAMETERS_FACTORY(Device1394);

    bool setFeatures(const FeatureSet & features);
    bool getFeatures(FeatureSet & features) const;

  protected:
    virtual BufferManager * bufferManagerInstance() const;
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void fini();
    
    //! Detect and initialize the camera.
    void initDevice(int port = 0);
    //! Close device-driver handles.
    void cleanupDevice();
	
    void setImageFormat();

    //! set extra (and device specific) camera parameters.
    void initSettings();
	
    //! Initialize dma transmission
    void initCapture();

    Device1394Parameters    params_;

  private:
    //! Flag to indicate that the device is initialized.
    bool                    is_open_;
    //! Real device handle.
    raw1394handle_t         handle_;
    //! Actual camera features.
    dc1394_feature_set      features_;
    //! Camera capture data structure.
    dc1394_cameracapture *  p_camera_;
    //! Selected image format.
    long                    imageFormat_;
    //! Framerate
    int                     frameRate_;
    //! Pointer to Control-Interface.
    ControlImpl *           control_;
  };
};
#endif // VIDEODEVICE1394_H

