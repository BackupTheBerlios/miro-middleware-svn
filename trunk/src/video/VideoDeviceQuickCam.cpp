// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#include <ace/OS.h>

#include "VideoDeviceQuickCam.h"
#include "BufferManagerQuickCam.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include <string>

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(DeviceQuickCam);

  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  int DeviceQuickCam::gb_pal[64];

  DeviceQuickCam::DeviceQuickCam(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    devName_(),
    ioBuffer_(),
    connector_(),
    channels(NULL),
    map_(NULL),
    currentBuffer_(0),
    nextBuffer_(0)
  {
    MIRO_LOG_CTOR("Device::QuickCam");

    /**
       cleanup supported format lookup table
    */
    for (int i = 0; i < 64; ++i)
      gb_pal[i] = 0;

  }

  DeviceQuickCam::~DeviceQuickCam()
  {
    MIRO_LOG_DTOR("Device::QuickCam");
  }


  bool
  DeviceQuickCam::setFeatures(const QuickCamFeatureSet &features) 
  {
    setFramerate(features.framerate);
    setVideoWindow();
    
    setBrightness(features.brightness);
    setHue(features.hue);
    setContrast(features.contrast);
    setVideoPicture();
    
    setWbMode(features.whitebalance);
    setWbManualRed(features.whitebalanceManualRed);
    setWbManualBlue(features.whitebalanceManualBlue);
    setWbSpeed(features.whitebalanceSpeed);
    setWbDelay(features.whitebalanceDelay);
    setAGC(features.autoGainControl);
    setShutterSpeed(features.shutterSpeed);
    setSharpness(features.sharpness);
    setBC(features.backlightCompensation);
    setFM(features.flickerlessMode);
    setDNR(features.dynamicNoiseReduction);
    setCompression(features.compression);
    setPWC();

    return true;
  }
  
  bool
  DeviceQuickCam::getFeatures(QuickCamFeatureSet &features) 
  {
    getVideoWindow();
    features.framerate = getFramerate();
   
    getVideoPicture();
    features.brightness = getBrightness();
    features.hue = getHue();
    features.contrast = getContrast();
   
    getPWC();
    features.whitebalance = getWbMode();
    features.whitebalanceManualRed = getWbManualRed();
    features.whitebalanceManualBlue = getWbManualBlue();
    features.whitebalanceReadRed = getWbReadRed();
    features.whitebalanceReadBlue = getWbReadBlue();
    features.whitebalanceSpeed = getWbSpeed();
    features.whitebalanceDelay = getWbDelay();
    features.autoGainControl = getAGC();
    features.shutterSpeed = getShutterSpeed();
    features.sharpness = getSharpness();
    features.backlightCompensation = getBC();
    features.flickerlessMode = getFM();
    features.dynamicNoiseReduction = getDNR();
    features.compression = getCompression();

    std::cout << "fps: " << features.framerate << std::endl
	      << "brightness: " << features.brightness << std::endl
	      << "hue: " << features.hue << std::endl
	      << "contrast: " << features.contrast << std::endl
	      << "whitebalance: " << features.whitebalance << std::endl
	      << "wb manual red: " << features.whitebalanceManualRed << std::endl
	      << "wb blue: " << features.whitebalanceManualBlue << std::endl
	      << "wb read red: " << features.whitebalanceReadRed << std::endl
	      << "wb read blue: " << features.whitebalanceReadBlue << std::endl
	      << "wb speed: " << features.whitebalanceSpeed << std::endl
	      << "wb delay: " << features.whitebalanceDelay << std::endl
	      << "gain control: " << features.autoGainControl << std::endl
	      << "shutter: " << features.shutterSpeed << std::endl
	      << "sharpness: " << features.sharpness << std::endl
	      << "backlichtCompensation: " << features.backlightCompensation << std::endl
	      << "flickerless mode: " << features.flickerlessMode << std::endl
	      << "dynamic noise red:" << features.dynamicNoiseReduction << std::endl
	      << "compression: " << features.compression  << std::endl;

    return true;
  }
  
  BufferManager *
  DeviceQuickCam::bufferManagerInstance() const
  {
    unsigned char * bufferAddr[gb_buffers.frames];

    for (int i = 0; i < gb_buffers.frames; ++i)
      bufferAddr[i] = map_ + gb_buffers.offsets[i];

    return new BufferManagerQuickCam(this,
				 ioBuffer_.get_handle(),
				 outputFormat_.width,
				 outputFormat_.height,
				 15,
				 gb_buffers.frames,
				 bufferAddr);
  }


  void
  DeviceQuickCam::init(Miro::Server& _server, FilterParameters const * _params)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Connecting DeviceQuickCam.");
   
    int	err = 0;
    params_ = dynamic_cast<DeviceQuickCamParameters const *>(_params);
    assert(params_ != NULL);

    devName_.set(params_->device.c_str());
    if (connector_.connect(ioBuffer_,
			  devName_,
			  0, ACE_Addr::sap_any, 0, O_RDONLY) == -1) {
      std::stringstream s;
      s << "Failed to open device: " << params_->device << std::endl
	<< "Propably running on the wrong machine?" << std::endl;
      throw Miro::CException(errno, s.str());
    }

    //fcntl(ioBuffer_.get_handle(), F_SETFD, FD_CLOEXEC);
    pwc_probe probe;
    if (ioctl(ioBuffer_.get_handle(), VIDIOCPWCPROBE, &probe) == 0) {
      MIRO_LOG_OSTR(LL_NOTICE,
		    "Probing Device: " << probe.name << " successfull.");
    } 
    else {
       throw Miro::CException(errno, "Error Probing Device QuickCam");
    }

    /* video capabilities */
    getCapabilities();

    /* video channels aka inputs */
    getChannels();

    /* set video channel */

    setChannel();

    if (capability.type & VID_TYPE_CAPTURE) {
      err = ioctl(ioBuffer_.get_handle(), VIDIOCGMBUF, &gb_buffers);
      if (err == -1)
	throw Miro::CException(errno, "DeviceQuickCam::handleConnect() - VIDIOCGMBUF");
      map_ = (unsigned char*)mmap(0, gb_buffers.size, PROT_READ, MAP_SHARED, ioBuffer_.get_handle(), 0);
      if ((int)map_ == -1)
	throw Miro::CException(errno, "DeviceQuickCam::handleConnect() - mmap()");
    }

    iNBuffers = gb_buffers.frames;

    MIRO_LOG_OSTR(LL_NOTICE,
	     "Buffersize: " << gb_buffers.size << std::endl
	     << "Buffersize/Frame: " << gb_buffers.size/gb_buffers.frames << std::endl
	     << "frames: " << gb_buffers.frames);

    probeAllFormats();

    QuickCamFeatureSet s;
    getFeatures(s);

    //setFormat();
    //setSource();
    setPalette();


    
    getVideoWindow();

    setFramerate(params_->framerate);

    setVideoWindow();
    getVideoWindow();

    /* set parameters */
    MIRO_DBG(VIDEO, LL_DEBUG, "Retrieving video_picture information.");

    getVideoPicture();

    setBrightness(params_->brightness);
    setHue(params_->hue);
    setContrast(params_->contrast);

    setVideoPicture();
    /* set pwc centric parameters */
    MIRO_DBG(VIDEO, LL_DEBUG, "Retrieving PWC information.");
//    getPWC();
#ifdef oaisfdh
    setWbMode(params_->whitebalance);
    setWbManualRed(params_->whitebalanceRed);
    setWbManualBlue(params_->whitebalanceBlue);
    setWbSpeed(params_->whitebalanceSpeed);
    setWbDelay(params_->whitebalanceDelay);
    setAGC(params_->autoGainControl);
    setShutterSpeed(params_->shutterSpeed);
    setSharpness(params_->sharpness);
    setBC(params_->backlightCompensation);
    setFM(params_->flickerlessMode);
    setDNR(params_->dynamicNoiseReduction);
    setCompression(params_->compression);
    setPWC();
#endif
    //	preparing buffers

/*    delete gb;
    gb = new struct video_mmap[gb_buffers.frames];

    for (int i = 0; i < gb_buffers.frames; ++i) {
      gb[i].width  = outputFormat_.width;
      gb[i].height = outputFormat_.height;
      gb[i].format = 15;
      gb[i].frame = i;
    }

    err = ioctl(ioBuffer_.get_handle(), VIDIOCMCAPTURE, &(gb[nextBuffer_]));
    ++nextBuffer_; */

    Super::init(_server, _params);

  }

  void DeviceQuickCam::fini()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device::QuickCam::fini().");

    delete[] channels;
    channels = NULL;

    if ((int)map_ != -1 && map_ != NULL) {
      munmap(map_, gb_buffers.size);
      map_ = NULL;
    }
    ioBuffer_.close();

    // removing buffer ptr
    //buffer_ = NULL;
  }


  void DeviceQuickCam::setPalette()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceQuickCam::setPalette()");

    outputFormat_.palette = Miro::YUV_420P;

  }

  void DeviceQuickCam::getCapabilities()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceQuickCam: getCapabilities()");

    int	err = ioctl(ioBuffer_.get_handle(), VIDIOCGCAP, &capability);
    if (err == -1)
      throw Miro::CException(errno, "DeviceQuickCam::getCapabilities() - VIDIOCAP");
  }


  void DeviceQuickCam::getChannels()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceQuickCam::getChannels()");

    /* input sources */
    channels = new struct video_channel[capability.channels];
    for (int i = 0; i < capability.channels; i++) {
      channels[i].channel = i;
      int err = ioctl(ioBuffer_.get_handle(), VIDIOCGCHAN, &channels[i]);
      if (err == -1)
	throw Miro::CException(errno, "DeviceQuickCam::getChannels() - VIDIOCGCHAN");
    }
  }

  void DeviceQuickCam::setChannel()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceQuickCam::setChannel()");

    int err = ioctl(ioBuffer_.get_handle(), VIDIOCSCHAN, &(channels[0]));
    if (err < 0)
       throw Miro::CException(errno, "DeviceQuickCam::setChannel() - VIDIOCSCHAN");
  }

  bool
  DeviceQuickCam::probeFormat(int format)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceQuickCam: probeFormat()");

    struct video_mmap gb;
    int	err;

    if (0 != gb_pal[format])
      goto done;

    gb.frame  = 0;
    gb.width  = 64;
    gb.height = 48;
    gb.format = format;

    err = ioctl(ioBuffer_.get_handle(), VIDIOCMCAPTURE, &gb);
    if (err == -1) {
      gb_pal[format] = 2;
      goto done;
    }

    err = ioctl(ioBuffer_.get_handle(), VIDIOCSYNC, &gb);
    if (err == -1) {
      gb_pal[format] = 3;
      goto done;
    }

    gb_pal[format] = 1;

  done:
    return gb_pal[format] == 1;
  }

  int
  DeviceQuickCam::probeAllFormats()
  {
    int	count = 0;
    for (int i = 0; i < 64; ++i) {
      if (probeFormat(i))
	++count;
    }
    return count;
  }


  void
  DeviceQuickCam::getVideoWindow() 
  {
    ioctl(ioBuffer_.get_handle(), VIDIOCGWIN, &vidwindow);
        MIRO_DBG_OSTR(VIDEO, LL_DEBUG, "fps " << (vidwindow.flags >> 16));
  }

  
  void
  DeviceQuickCam::setVideoWindow() 
  {
    MIRO_DBG_OSTR(VIDEO, LL_DEBUG, "setting " << (vidwindow.flags >> 16));
    ioctl(ioBuffer_.get_handle(), VIDIOCSWIN, &vidwindow);
  }
  

  unsigned int
  DeviceQuickCam::getFramerate() {
    return vidwindow.flags >> 16;
  }

  
  void
  DeviceQuickCam::setFramerate(unsigned int _framerate) {
    vidwindow.flags = _framerate << 16;
  }
  

  void
  DeviceQuickCam::getVideoPicture() {
    ioctl(ioBuffer_.get_handle(), VIDIOCGPICT, &vidpicture);
  }

  void
  DeviceQuickCam::setVideoPicture() {
    ioctl(ioBuffer_.get_handle(), VIDIOCSPICT, &vidpicture);
  }

  
  unsigned short
  DeviceQuickCam::getBrightness() {
    return vidpicture.brightness;
  }

  
  void
  DeviceQuickCam::setBrightness(unsigned short _brightness) {
    vidpicture.brightness = _brightness;
  }

  
  unsigned short
  DeviceQuickCam::getHue() {
    return vidpicture.hue;
  }

  
  void
  DeviceQuickCam::setHue(unsigned short _hue) {
    vidpicture.hue = _hue;
  }
  

  unsigned short
  DeviceQuickCam::getContrast() {
    return vidpicture.contrast;
  }

  
  void
  DeviceQuickCam::setContrast(unsigned short _contrast) {
    vidpicture.contrast = _contrast;
  }


  void
  DeviceQuickCam::getPWC() {
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCGAWB, &pwcWb);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCGAWBSPEED, &pwcWbSpeed);
    pwcShutterSpeed = 0;
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCGAGC, &pwcAGC);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCGCONTOUR, &pwcSharpness);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCGBACKLIGHT, &pwcBC);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCGFLICKER, &pwcFM);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCGDYNNOISE, &pwcDNR);
  }

  
  void
  DeviceQuickCam::setPWC() {
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCSDYNNOISE, &pwcDNR);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCSFLICKER, &pwcFM);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCSBACKLIGHT, &pwcBC);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCSCONTOUR, &pwcSharpness);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCSAGC, &pwcAGC);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCSSHUTTER, &pwcShutterSpeed);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCSAWBSPEED, &pwcWbSpeed);
    ioctl(ioBuffer_.get_handle(), VIDIOCPWCSAWB, &pwcWb);
  }

  
  int
  DeviceQuickCam::getWbMode() {
    return pwcWb.mode;
  }

  
  void
  DeviceQuickCam::setWbMode(int _mode) {
    pwcWb.mode = _mode;
  }

  
  int
  DeviceQuickCam::getWbReadRed() {
    return pwcWb.read_red;
  }

  
  int
  DeviceQuickCam::getWbManualRed() {
    return pwcWb.manual_red;
  }

  
  void
  DeviceQuickCam::setWbManualRed(int _red) {
    pwcWb.manual_red = _red;
  }
  

  int
  DeviceQuickCam::getWbReadBlue() {
    return pwcWb.read_blue;
  }

  
  int
  DeviceQuickCam::getWbManualBlue() {
    return pwcWb.manual_blue;
  }

  
  void
  DeviceQuickCam::setWbManualBlue(int _blue) {
    pwcWb.manual_blue = _blue;
  }

  
  int
  DeviceQuickCam::getWbSpeed() {
    return pwcWbSpeed.control_speed;
  }

  
  void
  DeviceQuickCam::setWbSpeed(int _speed) {
    pwcWbSpeed.control_speed = _speed;
  }
  

  int
  DeviceQuickCam::getWbDelay() {
    return pwcWbSpeed.control_delay;
  }

  
  void
  DeviceQuickCam::setWbDelay(int _delay) {
    pwcWbSpeed.control_delay = _delay;
  }
  
  
  int
  DeviceQuickCam::getAGC() {
    return pwcAGC;
  }

  
  void
  DeviceQuickCam::setAGC(int _agc) {
    pwcAGC = _agc;
  }
  
   
  int
  DeviceQuickCam::getShutterSpeed() {
    return pwcShutterSpeed;
  }

  
  void
  DeviceQuickCam::setShutterSpeed(int _shutterSpeed) {
    pwcShutterSpeed = _shutterSpeed;
  }
  
    
  int
  DeviceQuickCam::getSharpness() {
    return pwcSharpness;
  }

  
  void
  DeviceQuickCam::setSharpness(int _sharpness) {
    pwcSharpness = _sharpness;
  }
  

  int
  DeviceQuickCam::getBC() {
    return pwcBC;
  }

  
  void
  DeviceQuickCam::setBC(int _bc) {
    pwcBC = _bc;
  }
  

  int
  DeviceQuickCam::getFM() {
    return pwcFM;
  }

  
  void
  DeviceQuickCam::setFM(int _fm) {
    pwcFM = _fm;
  }
  

  int
  DeviceQuickCam::getDNR() {
    return pwcDNR;
  }

  
  void
  DeviceQuickCam::setDNR(int _dnr) {
    pwcDNR = _dnr;
  }
    

  int
  DeviceQuickCam::getCompression() {
    return pwcCompression;
  }

  
  void
  DeviceQuickCam::setCompression(int _compression) {
    pwcCompression = _compression;
  } 
}
