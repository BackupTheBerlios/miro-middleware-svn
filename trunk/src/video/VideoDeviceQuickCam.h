// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoDeviceQuickCam_h
#define VideoDeviceQuickCam_h

extern "C" {
#include <linux/videodev.h>
#include "pwc-ioctl.h"
}


#include "VideoDevice.h"
#include "idl/QuickCamControlC.h"

#include <ace/DEV_Connector.h>

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  class DeviceQuickCam : public Device
  {
    typedef Device Super;
  public:

    DeviceQuickCam(const Miro::ImageFormatIDL& _inputFormat);
    virtual ~DeviceQuickCam();

    FILTER_PARAMETERS_FACTORY(DeviceQuickCam);

    bool setFeatures(const Miro::QuickCamFeatureSet &features);
    bool getFeatures(Miro::QuickCamFeatureSet &features);

  protected:

    virtual BufferManager * bufferManagerInstance() const;
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void fini();

    // methods
    void setPalette();
    void setSize();

    void getCapabilities();
    void getChannels();
    void setChannel();

    void getVideoWindow();
    void setVideoWindow();

    unsigned int getFramerate();
    void setFramerate(unsigned int _framerate);

    void getVideoPicture(); 
    void setVideoPicture();

    unsigned short getBrightness();
    void setBrightness(unsigned short _brightness);

    unsigned short getHue();
    void setHue(unsigned short _hue);

    unsigned short getContrast();
    void setContrast(unsigned short _contrast);

    void getPWC();
    void setPWC();

    int getWbMode();
    void setWbMode(int _mode);

    int getWbReadRed();
    int getWbManualRed();
    void setWbManualRed(int _red);

    int getWbReadBlue();
    int getWbManualBlue();
    void setWbManualBlue(int _blue);

    int getWbSpeed();
    void setWbSpeed(int _speed);

    int getWbDelay();
    void setWbDelay(int _delay);
    
    int getAGC();
    void setAGC(int _agc);
   
    int getShutterSpeed();
    void setShutterSpeed(int _shutterSpeed);
    
    int getSharpness();
    void setSharpness(int _sharpness);

    int getBC();
    void setBC(int _bc);

    int getFM();
    void setFM(int _fm);

    int getDNR();
    void setDNR(int _dnr);
    
    int getCompression();
    void setCompression(int _compression);
    
    bool probeFormat(int format);
    int probeAllFormats();

    // static methods

    // data
    DeviceQuickCamParameters const * params_;

    ACE_DEV_Addr devName_;
    ACE_DEV_IO ioBuffer_;
    ACE_DEV_Connector connector_;

//    mutable struct video_mmap * gb;
    struct video_mbuf gb_buffers;
    struct video_capability capability;
    struct video_channel * channels;
    struct video_window vidwindow;
    struct video_picture vidpicture;

    /* pwc centric */
    struct pwc_whitebalance pwcWb;
    struct pwc_wb_speed pwcWbSpeed;
    int pwcAGC; /* auto gain control */
    int pwcShutterSpeed;
    int pwcSharpness;
    int pwcBC; /* backlight compensation */
    int pwcFM; /* flickerless mode */
    int pwcDNR; /* dynamic noise reduction */
    int pwcCompression;

    unsigned char * map_;

    int		iNBuffers;
    mutable int	iCurrentBuffer;

    mutable int currentBuffer_;
    mutable int nextBuffer_;

    // static protected data
    //! screen grab
    static int gb_pal[64];
  };
}
#endif
