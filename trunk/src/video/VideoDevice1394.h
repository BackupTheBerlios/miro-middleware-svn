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

#if defined(HAVE_LIBDC1394_CONTROL) && defined(HAVE_LIBRAW1394)
#define HAVE_VIDEODEVICE1394 1
#endif

#ifdef HAVE_VIDEODEVICE1394

#include <libraw1394/raw1394.h>
#include <libdc1394/dc1394_control.h>

namespace Video
{
  class VideoConverter;

  //! Firewire camera support.
  class VideoDevice1394 : public VideoDevice
  {
    typedef VideoDevice Super;
    
  public:
    VideoDevice1394(Parameters const * _params = Parameters::instance());
    virtual ~VideoDevice1394();
	
    virtual void connect();
    virtual void disconnect();
    
    virtual void * grabImage(ACE_Time_Value& _timeStamp) const;

  protected:
    //! Detect and initialize the camera.
    void initDevice(int port = 0);
    //! Close device-driver handles.
    void cleanupDevice();
	
    void setFormat(VideoFormat fmt);
    void setSource(VideoSource src);
    void setPalette(VideoPalette pal);
    void setSubfield(VideoSubfield sub);
    void setSize(int w, int h);

    //! set extra (and device specific) camera parameters.
    void initSettings(const Video::FireWireParameters & _params);
	
    //! Allocate and setup the framebuffer memory.
    void initBuffers(int buf_cnt);
	
    //! Initialize dma transmission
    void initCapture();

  private:
    VideoConverter *        converter_;

    //! Flag to indicate that the device is initialized.
    bool                    is_open_;
    //! Real device handle.
    raw1394handle_t         handle_;
    //! Actual camera features.
    dc1394_feature_set      features_;
    //! Camera capture data structure.
    dc1394_cameracapture *  p_camera_;
    //! Selected image format.
    long                    image_format_;
    //! Pointer to framebuffer memory.
    char *                  frame_buffers_;
    //! Framerate
    int                     frame_rate_;
  };
};

#endif // HAVE_VIDEODEVICE1394

#endif // VIDEODEVICE1394_H

