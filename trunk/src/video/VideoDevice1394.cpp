/* -*- c++ -*- */
/*! \file VideoDevice1394.cpp
 *  \brief Implementation for a firewire video device
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
 * Revision 1.13  2004/03/11 12:04:47  hutz
 * cleaning up debug output for filter framework, firewire and quickcam
 * BTTV and Meteor are left dirty for now.
 *
 * Revision 1.12  2004/02/09 17:28:56  graz
 * Added control-interface to VideoDevice1394 & Dialog.
 *
 * Revision 1.11  2003/10/27 08:52:41  hutz
 * added the filter meta information functionality:
 * for each buffer we have an additional pointer, holding a child
 * of the FilterImageParameters struct. Accessor methods within the filter
 * class do exist. The filter parameters are generated by the well known
 * parameters framework
 *
 * Revision 1.10  2003/10/17 13:31:42  hutz
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
 * Revision 1.9  2003/06/03 13:36:27  hutz
 * trying to remove segfaults on shutdown
 *
 * Revision 1.8  2003/06/03 11:03:36  hutz
 * fixed copy paste error in macros.miro.GNU
 * removed the HAVE_* macros in VideoDevice1394.*
 *
 * Revision 1.7  2003/06/03 10:25:32  hutz
 * complete revamp of the video service
 * the interface changes slightly to allow for better access
 * removed unnecessary copies
 * added a complete filter framework for server side image processing
 * added a library miroVideo.a for easy customization to own video service
 * derivates
 * the dummy video device now displays an image
 *
 * Revision 1.6  2003/05/16 13:14:57  hutz
 * removing unused parameters and methods from VideoDevice
 *
 * Revision 1.5  2003/05/13 21:58:49  hutz
 * removing the bridge pattern between VideoDevice and VideoDeviceBase
 * making VideoDevice* a direct descendant of VideoDevice
 *
 * Revision 1.4  2003/05/13 20:50:20  hutz
 * cleaning up the video service, getting rid of VideoConnection
 *
 * Revision 1.3  2003/05/13 10:56:34  hutz
 * debugging fire wire support
 *
 * Revision 1.2  2003/05/12 16:34:35  hutz
 * bug hunt on fire wire
 *
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.8  2003/03/11 18:19:16  muehlenf
 * Added device-parameter.
 *
 * Revision 1.7  2003/03/10 10:25:20  muehlenf
 * Corrected reading of the white-balance.
 *
 * Revision 1.6  2003/03/10 07:40:34  muehlenf
 * Additional parameters for firewire cameras.
 *
 * Revision 1.5  2003/01/20 10:54:17  muehlenf
 * Interface-test for the VideoService (using VideoDeviceDummy).
 *
 * Revision 1.4  2003/01/14 10:19:53  muehlenf
 * Started using configure & automake.
 *
 * Revision 1.3  2003/01/13 00:12:27  muehlenf
 * Some minor modifications.
 *
 * Revision 1.2  2003/01/10 13:46:46  muehlenf
 * Some corrections and tests, first working version.
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#include <ace/OS.h>

#include "VideoDevice1394.h"
#include "BufferManager1394.h"
#include "VideoControlImpl.h"

#include <miro/Exception.h>
#include <miro/Server.h>
#include <miro/Log.h>

#define NUM_BUFFERS 4
#define DROP_FRAMES 1

namespace
{
  //---------------------------------------------------------------
  struct Mode1394
  {
    Miro::ImageFormatIDL format;
    int mode;
  };

  static Mode1394 mode1394[NUM_FORMAT0_MODES] = {
    { {160, 120, Miro::YUV_24 }, MODE_160x120_YUV444},
    { {320, 240, Miro::YUV_422}, MODE_320x240_YUV422},
    { {640, 480, Miro::YUV_411}, MODE_640x480_YUV411},
    { {640, 480, Miro::YUV_422}, MODE_640x480_YUV422},
    { {640, 480, Miro::RGB_24 }, MODE_640x480_RGB},
    { {640, 480, Miro::GREY_8 }, MODE_640x480_MONO},
    { {640, 480, Miro::GREY_16}, MODE_640x480_MONO16}
  };

  //---------------------------------------------------------------
  struct Feature1394
  {
    char const * name;
    int const * pValue;
    int code;
    int (* set_function) (raw1394handle_t handle,
			  nodeid_t node,
			  unsigned int sharpness);
  };

  //---------------------------------------------------------------
  static Feature1394 feature1394[12] = {
    { "brightness",  NULL, FEATURE_BRIGHTNESS,  dc1394_set_brightness },
    { "exposure",    NULL, FEATURE_EXPOSURE,    dc1394_set_exposure },
    { "focus",       NULL, FEATURE_FOCUS,	dc1394_set_focus },
    { "gain",        NULL, FEATURE_GAIN,	dc1394_set_gain },
    { "gamma",       NULL, FEATURE_GAMMA,	dc1394_set_gamma },
    { "hue",         NULL, FEATURE_HUE,         dc1394_set_hue },
    { "iris",        NULL, FEATURE_IRIS,	dc1394_set_iris },
    { "saturation",  NULL, FEATURE_SATURATION,  dc1394_set_saturation },
    { "sharpness",   NULL, FEATURE_SHARPNESS,   dc1394_set_sharpness },
    { "shutter",     NULL, FEATURE_SHUTTER,     dc1394_set_shutter },
    { "temperature", NULL, FEATURE_TEMPERATURE, dc1394_set_temperature },
    { "trigger",     NULL, FEATURE_TRIGGER,     dc1394_set_trigger_mode }
  };

  //---------------------------------------------------------------
  struct FeatureTable
  {
    static const unsigned int NUM_FEATURES1394 = 12;

    FeatureTable(const Video::Device1394Parameters& _params) : 
      feature_(	feature1394 ) 
    {
      feature_[0].pValue = & (_params.brightness);
      feature_[1].pValue = & (_params.exposure);
      feature_[2].pValue = & (_params.focus);
      feature_[3].pValue = & (_params.gain);
      feature_[4].pValue = & (_params.gamma);
      feature_[5].pValue = & (_params.hue);
      feature_[6].pValue = & (_params.iris);
      feature_[7].pValue = & (_params.saturation);
      feature_[8].pValue = & (_params.sharpness);
      feature_[9].pValue = & (_params.shutter);
      feature_[10].pValue = & (_params.temperature);
      feature_[11].pValue = & (_params.trigger);
    }
    const Feature1394& operator[] (unsigned int _index) { 
      return feature_[_index]; 
    }
    Feature1394 const * begin() const { 
      return &feature_[0]; 
    }
    Feature1394 const * end() const { 
      return &feature_[NUM_FEATURES1394]; 
    }

  protected:
    Feature1394  * feature_;
  };
};

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(Device1394);

  //---------------------------------------------------------------
  Device1394::Device1394(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    is_open_(false),
    handle_(0),
    p_camera_(new dc1394_cameracapture()),
    frameRate_(FRAMERATE_30)
  {
    MIRO_LOG_CTOR("Video::Device1394.");

    paletteLookup[Miro::GREY_8] = 1;
    paletteLookup[Miro::GREY_16] = 1;
    paletteLookup[Miro::RGB_24] = 1;
    paletteLookup[Miro::YUV_411] = 1;
    paletteLookup[Miro::YUV_422] = 1;
  }
    
  //---------------------------------------------------------------
  Device1394::~Device1394()
  {
    MIRO_LOG_DTOR("Video::Device1394.");
    delete p_camera_;
  } 

  BufferManager * 
  Device1394::bufferManagerInstance() const 
  {
    return new BufferManager1394(this, p_camera_);
  }

  //---------------------------------------------------------------
  void
  Device1394::init(Miro::Server& _server, FilterParameters const * _params)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::init()");

    if (_params)
        params_ = *(dynamic_cast<Device1394Parameters const *>(_params));

    initDevice();
    initSettings();
	
    setImageFormat();
    initCapture();

    control_ = new ControlImpl(this);
    Video::Control_var control = control_->_this();
    _server.addToNameService(control.in(), "VideoControl");

    Super::init(_server, _params);
  }

  //---------------------------------------------------------------
  void
  Device1394::fini()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::fini()");
	
    if (is_open_)
    {
      MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::fini() close");
      dc1394_stop_iso_transmission(handle_, p_camera_->node);
      dc1394_dma_unlisten(handle_, p_camera_);
      dc1394_dma_release_camera(handle_, p_camera_);
      is_open_ = false;
    }

    cleanupDevice();
  }
    
  //---------------------------------------------------------------
  void
  Device1394::initDevice(int port)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::initDevice()");

    handle_ = dc1394_create_handle(port);
    if (!handle_) {
      throw Miro::Exception("Device1394::initDevice: unable to create handle");
    }

    int num_nodes = raw1394_get_nodecount(handle_);
    int camera_count = 0;
    nodeid_t * camera_nodes = dc1394_get_camera_nodes(handle_, &camera_count, 1);

    MIRO_DBG_OSTR(VIDEO, LL_DEBUG,
		  "Device1394::initDevice: cameracount = " << camera_count);
    if (camera_count < 1)
      throw Miro::Exception("Device1394::initDevice: no camera found");

    p_camera_->node = camera_nodes[0];
    MIRO_DBG_OSTR(VIDEO, LL_DEBUG, 
		  "Device1394::initDevice: node = " << 
		  p_camera_->node << "/" << num_nodes);
    if (p_camera_->node == num_nodes - 1)
      throw Miro::Exception("Device1394::initDevice: camera is highest numbered node.");

    dc1394_stop_iso_transmission(handle_, p_camera_->node);

    if (dc1394_get_camera_feature_set(handle_, p_camera_->node, &features_) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::initDevice: unable to get camera features");
	
    if (Miro::Log::level() >= Miro::Log::LL_NOTICE)
      dc1394_print_feature_set(&features_);
  }

  //---------------------------------------------------------------
  void 
  Device1394::cleanupDevice()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::cleanupDevice()");

    if (handle_) {
      dc1394_destroy_handle(handle_);
      handle_ = 0;
    }
  }

  //---------------------------------------------------------------
  void
  Device1394::setImageFormat()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::setImageFormat()");

    unsigned int i;
    for (i = 0; i < NUM_FORMAT0_MODES; ++i) {
      if (inputFormat_.width  == mode1394[i].format.width &&
	  inputFormat_.height == mode1394[i].format.height &&
	  inputFormat_.palette == mode1394[i].format.palette ) {
	imageFormat_ = mode1394[i].mode;
	break;
      }
    }
    if (i == NUM_FORMAT0_MODES)
      throw Miro::Exception("Device1394::setImageFormat - Unsupported image format requested.");
  }

  //---------------------------------------------------------------
  void
  Device1394::initCapture()
  {
    unsigned int channel;
    unsigned int speed;

    if (dc1394_get_iso_channel_and_speed(handle_, p_camera_->node, &channel, &speed) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394:initDevice: unable to get iso channel number");
    MIRO_DBG_OSTR(VIDEO, LL_DEBUG,
		  "ISO channel: " << channel << "    speed: " << speed);
    channel = 1;

    if (dc1394_dma_setup_capture(handle_,
				 p_camera_->node,
				 channel,
				 FORMAT_VGA_NONCOMPRESSED,
				 imageFormat_,
				 SPEED_400,
				 frameRate_,
				 NUM_BUFFERS,
				 DROP_FRAMES,
				 params_.device.c_str(),
				 p_camera_) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::handleConnect: unable to setup camera");
	
    if (dc1394_start_iso_transmission(handle_, p_camera_->node) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::handleConnect: unable to start camera iso transmission");
    is_open_ = true;
  }

  //---------------------------------------------------------------
  void
  Device1394::initSettings()
  {
    // set standardized features
    FeatureTable featureTable(params_);
    Feature1394 const * first;
    Feature1394 const * last = featureTable.end();
    for (first = featureTable.begin(); first != last; ++first) {
      if (*(first->pValue) == -1) {
	dc1394_auto_on_off(handle_, p_camera_->node, first->code, 1);
      }
      else if (*(first->pValue) >= 0) {
	if (first->set_function(handle_,
				p_camera_->node,
				(*first->pValue)) != DC1394_SUCCESS) {
	  MIRO_LOG_OSTR(LL_WARNING,
			"Feature " << 
			first->name << " = " << (*first->pValue) <<
			" not set.");
	}
	dc1394_auto_on_off(handle_, p_camera_->node, first->code, 0);
      }
    }

    // white balance
    if (params_.whiteBalance0 == -1 ||
	params_.whiteBalance1 == -1) {
      dc1394_auto_on_off(handle_, p_camera_->node, FEATURE_WHITE_BALANCE, 1);
    }
    else if (params_.whiteBalance0 >= 0 &&
	     params_.whiteBalance1 >= 0) {
      if (dc1394_set_white_balance(handle_,
				   p_camera_->node,
				   params_.whiteBalance0,
				   params_.whiteBalance1) != DC1394_SUCCESS) {
	  MIRO_LOG_OSTR(LL_WARNING,
			"Feature WhiteBalance = " <<
			params_.whiteBalance0 << ":" << params_.whiteBalance1 <<
			" not set.");
      }
      dc1394_auto_on_off(handle_, p_camera_->node, FEATURE_WHITE_BALANCE, 0);
    }

    // set framerate
    int value = params_.framerate;
    if (value <= 1)	   
      frameRate_ = FRAMERATE_1_875;
    else if (value <= 3)
      frameRate_ = FRAMERATE_3_75;
    else if (value <= 7)
      frameRate_ = FRAMERATE_7_5;
    else if (value <= 15)
      frameRate_ = FRAMERATE_15;
    else
      frameRate_ = FRAMERATE_30;
  }
//---------------------------------------------------------------
    bool Device1394::setFeatures(const FeatureSet & features)
    {
        params_.brightness = features.brightness;
        params_.exposure = features.exposure;
        params_.focus = features.focus;
        params_.gain = features.gain;
        params_.gamma = features.gamma;
        params_.hue = features.hue;
        params_.iris = features.iris;
        params_.saturation = features.saturation;
        params_.sharpness = features.sharpness;
        params_.shutter = features.shutter;
        params_.temperature = features.temperature;
        params_.trigger = features.trigger;
	params_.whiteBalance0 = features.white_balance.first;
	params_.whiteBalance1 = features.white_balance.second;
	initSettings();
        return true;
    }
    
//---------------------------------------------------------------
    bool Device1394::getFeatures(FeatureSet & features) const
    {
        features.brightness = params_.brightness;
        features.exposure = params_.exposure;
        features.focus = params_.focus;
        features.gain = params_.gain;
        features.gamma = params_.gamma;
        features.hue = params_.hue;
        features.iris = params_.iris;
        features.saturation = params_.saturation;
        features.sharpness = params_.sharpness;
        features.shutter = params_.shutter;
        features.temperature = params_.temperature;
        features.trigger = params_.trigger;
	features.white_balance.first = params_.whiteBalance0;
	features.white_balance.second = params_.whiteBalance1;
        return true;
    }

};

