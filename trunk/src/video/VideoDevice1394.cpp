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
 * Revision 1.18  2006/02/03 08:40:05  gmayer
 * apply patch from Sean Verret
 * description taken from his email see below
 *
 *
 * I made the changes to The ULM code to make the flea camera work.  Here
 * is a summary of the changes you can pass on to Ulm:
 *
 * 1. I added support for FORMAT1 type cameras.  This includes the
 * MODE_1024x768_MONO format that we are interested in.
 *
 * 2. I added support for SVGA_NONCOMPRESSED_1 video Format
 *
 * In order to accomplish the above tasks I defined two new variables
 * videoFormat and videoMode in the Device1394 section of the parameters
 * file.  These variables are used in VideoDevice1394 to pass the correct
 * parameters to libdc.
 *
 * videoFormat is a string.  Valid entries are:
 * VGA (This equates to the previous Ulm code)
 * SVGA1 This adds support for the flea camera
 *
 * videoMode is an integre.  Valid entries are:
 * 0 - (Format0 cameras handle the previous Ulm code)
 * 1 - (Format1 handles 800x600 and 1024x768 cameras)
 *
 * Revision 1.17  2005/11/17 15:58:31  gmayer
 * now works with libdc1394 version 2 (tested with 2.0.0-pre5)
 * it's better to have this in place before it's the default verison in debian :-)
 *
 * Revision 1.16  2004/09/29 16:23:23  hutz
 * minor include cleanup
 *
 * Revision 1.15  2004/05/25 13:04:04  gmayer
 * oehm, compiler directives seems to have problems if they are defined with strings instead of numbers. changed this.
 *
 * BOOTSTRAP NECESSARY
 *
 * Revision 1.14  2004/05/19 14:50:26  gmayer
 * YACT -- yet another configure test
 * because there is a change in libdc1394 0.9.4 we need to check which interface we have to use....
 *
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
 
#include "VideoDevice1394.h"
#include "BufferManager1394.h"
#include "VideoControlImpl.h"

#include <miro/Exception.h>
#include <miro/Server.h>
#include <miro/Log.h>

#include <ace/OS.h>

#define NUM_BUFFERS 4
#define DROP_FRAMES 1

#include <iostream>

namespace
{

  //---------------------------------------------------------------
  struct Mode1394
  {
    Miro::ImageFormatIDL format;
    int mode;
  };

  // mapping of miro formats to libdc1394 formats
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2

  static Mode1394 mode1394[NUM_FORMAT0_MODES] = {
    { {160, 120, Miro::YUV_24 }, MODE_160x120_YUV444},
    { {320, 240, Miro::YUV_422}, MODE_320x240_YUV422},
    { {640, 480, Miro::YUV_411}, MODE_640x480_YUV411},
    { {640, 480, Miro::YUV_422}, MODE_640x480_YUV422},
    { {640, 480, Miro::RGB_24 }, MODE_640x480_RGB},
    { {640, 480, Miro::GREY_16}, MODE_640x480_MONO16}
	};
    	
	static Mode1394 mode11394[NUM_FORMAT1_MODES] = {
    { {800, 600, Miro::YUV_422}, MODE_800x600_YUV422},
    { {1024, 768, Miro::YUV_422}, MODE_1024x768_YUV422},
    { {800, 600, Miro::RGB_24 }, MODE_800x600_RGB},
    { {1024, 768, Miro::RGB_24 }, MODE_1024x768_RGB},
    { {800, 600, Miro::GREY_8}, MODE_800x600_MONO},
    { {1024, 768, Miro::GREY_8}, MODE_1024x768_MONO},
    { {800, 600, Miro::GREY_16}, MODE_800x600_MONO16}
    };
#else
  static Mode1394 mode1394[DC1394_MODE_FORMAT0_NUM] = {
    { {160, 120, Miro::YUV_24 }, DC1394_MODE_160x120_YUV444},
    { {320, 240, Miro::YUV_422}, DC1394_MODE_320x240_YUV422},
    { {640, 480, Miro::YUV_411}, DC1394_MODE_640x480_YUV411},
    { {640, 480, Miro::YUV_422}, DC1394_MODE_640x480_YUV422},
    { {640, 480, Miro::RGB_24 }, DC1394_MODE_640x480_RGB8},
    { {640, 480, Miro::GREY_8 }, DC1394_MODE_640x480_MONO8},
    { {640, 480, Miro::GREY_16}, DC1394_MODE_640x480_MONO16}
	};
	
	static Mode1394 mode11394[DC1394_MODE_FORMAT1_NUM] = {
    { {800, 600, Miro::YUV_422}, DC1394_MODE_800x600_YUV422},
    { {1024, 768, Miro::YUV_422}, DC1394_MODE_1024x768_YUV422},
    { {800, 600, Miro::RGB_24 }, DC1394_MODE_800x600_RGB8},
    { {1024, 768, Miro::RGB_24 }, DC1394_MODE_1024x768_RGB8},
    { {800, 600, Miro::GREY_8}, DC1394_MODE_800x600_MONO8},
    { {1024, 768, Miro::GREY_8}, DC1394_MODE_1024x768_MONO8},
    { {800, 600, Miro::GREY_16}, DC1394_MODE_800x600_MONO16},
    { {1024, 768, Miro::GREY_16}, DC1394_MODE_1024x768_MONO16}
    };
#endif

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
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
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
#else
  static Feature1394 feature1394[12] = {
    { "brightness",  NULL, DC1394_FEATURE_BRIGHTNESS,  NULL },
    { "exposure",    NULL, DC1394_FEATURE_EXPOSURE,    NULL },
    { "focus",       NULL, DC1394_FEATURE_FOCUS,	NULL },
    { "gain",        NULL, DC1394_FEATURE_GAIN,	NULL },
    { "gamma",       NULL, DC1394_FEATURE_GAMMA,	NULL },
    { "hue",         NULL, DC1394_FEATURE_HUE,         NULL },
    { "iris",        NULL, DC1394_FEATURE_IRIS,	NULL },
    { "saturation",  NULL, DC1394_FEATURE_SATURATION,  NULL },
    { "sharpness",   NULL, DC1394_FEATURE_SHARPNESS,   NULL },
    { "shutter",     NULL, DC1394_FEATURE_SHUTTER,     NULL },
    { "temperature", NULL, DC1394_FEATURE_TEMPERATURE, NULL },
    { "trigger",     NULL, DC1394_FEATURE_TRIGGER,     NULL }
  };
#endif

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

using std::cout;
  using std::endl;
  FILTER_PARAMETERS_FACTORY_IMPL(Device1394);

  //---------------------------------------------------------------
  Device1394::Device1394(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    is_open_(false),
    handle_(0),
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    p_camera_(new dc1394_cameracapture()),
    frameRate_(FRAMERATE_30)
#else
    // p_camera_(new dc1394camera_t()),
      p_camera_(NULL),
    frameRate_(DC1394_FRAMERATE_30)
#endif


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
    // delete p_camera_;
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
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
      dc1394_stop_iso_transmission(handle_, p_camera_->node);
      dc1394_dma_unlisten(handle_, p_camera_);
      dc1394_dma_release_camera(handle_, p_camera_);
#else
      dc1394_video_set_transmission(p_camera_, DC1394_OFF);
      dc1394_dma_unlisten(p_camera_);
      dc1394_dma_release_camera(p_camera_);
#endif
      is_open_ = false;
    }

    cleanupDevice();
  }
    
  //---------------------------------------------------------------
  void
  Device1394::initDevice(int port)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::initDevice()");

#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2

    handle_ = dc1394_create_handle(port);
    if (!handle_) {
      throw Miro::Exception("Device1394::initDevice: unable to create handle");
    }

    int num_nodes = raw1394_get_nodecount(handle_);
    int camera_count = 0;
    nodeid_t * camera_nodes = dc1394_get_camera_nodes(handle_, &camera_count, 1);
    MIRO_DBG_OSTR(VIDEO, LL_DEBUG, "Device1394::initDevice: cameracount = " << camera_count);
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

#else

    dc1394camera_t ** cameras;
    unsigned int camera_count;
    dc1394_find_cameras(&cameras, &camera_count);
    MIRO_DBG_OSTR(VIDEO, LL_DEBUG, "Device1394::initDevice: cameracount = " << camera_count);
    if (camera_count < 1)
      throw Miro::Exception("Device1394::initDevice: no camera found");

    // for now, simply use the camera with the lowest number
    p_camera_=cameras[0];
    for (unsigned int i=1; i<camera_count; ++i) {
      dc1394_free_camera(cameras[i]);
    }
    free(cameras);

#endif
  }

  //---------------------------------------------------------------
  void 
  Device1394::cleanupDevice()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::cleanupDevice()");

#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    if (handle_) {
      dc1394_destroy_handle(handle_);
      handle_ = 0;
    }
#else
    if (p_camera_)
      dc1394_free_camera(p_camera_);
#endif
  }

  //---------------------------------------------------------------
  void
  Device1394::setImageFormat()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::setImageFormat()");

    unsigned int i;
	if( params_.videoMode == 0 )
	{
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    	for (i = 0; i < NUM_FORMAT0_MODES; ++i) 
		{
#else
    	for (i = 0; i < DC1394_MODE_FORMAT0_NUM; ++i) 
		{
#endif
      	if (inputFormat_.width  == mode1394[i].format.width &&
	  	inputFormat_.height == mode1394[i].format.height &&
	  	inputFormat_.palette == mode1394[i].format.palette ) {
		imageFormat_ = mode1394[i].mode;
		break;
      	}
    }
	
	#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    if (i == NUM_FORMAT0_MODES)
#else
    if (i == DC1394_MODE_FORMAT0_NUM)
#endif
      throw Miro::Exception("Device1394::setImageFormat - Unsupported image format requested.");
	
	}
	else if ( params_.videoMode == 1 )
	{
	#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    	for (i = 0; i < NUM_FORMAT1_MODES; ++i) 
		{
#else
    	for (i = 0; i < DC1394_MODE_FORMAT1_NUM; ++i) 
		{
#endif
      	if (inputFormat_.width  == mode11394[i].format.width &&
	  	inputFormat_.height == mode11394[i].format.height &&
	  	inputFormat_.palette == mode11394[i].format.palette ) {
		imageFormat_ = mode11394[i].mode;
		break;
      	}
    }
	
	#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    if (i == NUM_FORMAT1_MODES)
#else
    if (i == DC1394_MODE_FORMAT1_NUM)
#endif
      throw Miro::Exception("Device1394::setImageFormat - Unsupported image format requested.");
	} 
	else
	{
	   throw Miro::Exception("Device1394::setImageFormat - Unsupported params_.videoMode requested.");
	}


    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::setImageFormat() finished");
  }

  //---------------------------------------------------------------
  void
  Device1394::initCapture()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::initCapture()");

    unsigned int channel;
    unsigned int speed;
	unsigned int format;

#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    if (dc1394_get_iso_channel_and_speed(handle_, p_camera_->node, &channel, &speed) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394:initDevice: unable to get iso channel number");
#else
    if (dc1394_video_get_iso_channel_and_speed(p_camera_, &channel, &speed) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394:initDevice: unable to get iso channel number");
#endif
    MIRO_DBG_OSTR(VIDEO, LL_DEBUG, "ISO channel: " << channel << "    speed: " << speed);
    channel = 1;

#if ( LIBDC1394_VERSION == 1 )
	if ( params_.videoFormat == "SVGA1" )
	{
		format = FORMAT_SVGA_NONCOMPRESSED_1;
	}
	else if ( params_.videoFormat == "VGA" )
	{
			format = FORMAT_VGA_NONCOMPRESSED;
	}
	else
	{
		throw Miro::Exception("Device1394:initDevice: Invalid params_.videoFormat");
	}
	
#endif

#if ( LIBDC1394_VERSION == 1 )
    // old version
    if (dc1394_dma_setup_capture(handle_,
				 p_camera_->node,
				 channel,
				 format,
				 imageFormat_,
				 SPEED_400,
				 frameRate_,
				 NUM_BUFFERS,
				 DROP_FRAMES,
				 params_.device.c_str(),
				 p_camera_) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::handleConnect: unable to setup camera");
#elif ( LIBDC1394_VERSION == 2 )
    // new version
    if (dc1394_dma_setup_capture(handle_,
				 p_camera_->node,
				 channel,
				 format,
				 imageFormat_,
				 SPEED_400,
				 frameRate_,
				 NUM_BUFFERS,
				 1,
				 DROP_FRAMES,
				 params_.device.c_str(),
				 p_camera_) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::handleConnect: unable to setup camera");
#else
    // for version 2.x
    if (dc1394_dma_setup_capture(p_camera_,
				 channel,
				 imageFormat_,
				 DC1394_SPEED_400,
				 frameRate_, 
				 1,
				 DROP_FRAMES,
				 params_.device.c_str()) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::handleConnect: unable to setup camera");
#endif

#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
    if (dc1394_start_iso_transmission(handle_, p_camera_->node) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::handleConnect: unable to start camera iso transmission");
#else
    if (dc1394_video_set_transmission(p_camera_,  DC1394_ON) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::handleConnect: unable to start camera iso transmission");
#endif

    is_open_ = true;
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::initCapture() finished");
  }

  //---------------------------------------------------------------
  void
  Device1394::initSettings()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::initSettings()");

    // set standardized features
    FeatureTable featureTable(params_);
    Feature1394 const * first;
    Feature1394 const * last = featureTable.end();
    for (first = featureTable.begin(); first != last; ++first) {
      if (*(first->pValue) == -1) {
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
	dc1394_auto_on_off(handle_, p_camera_->node, first->code, 1);
#else
	dc1394_feature_set_mode(p_camera_, first->code, DC1394_FEATURE_MODE_AUTO);
#endif
      }
      else if (*(first->pValue) >= 0) {
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
	if (first->set_function(handle_,
				p_camera_->node,
				(*first->pValue)) != DC1394_SUCCESS)
	  MIRO_LOG_OSTR(LL_WARNING,
			"Feature " << 
			first->name << " = " << (*first->pValue) <<
			" not set.");
	dc1394_auto_on_off(handle_, p_camera_->node, first->code, 0);
#else
	if (dc1394_feature_set_value(p_camera_, first->code, (*first->pValue)) != DC1394_SUCCESS)
	  MIRO_LOG_OSTR(LL_WARNING,
			"Feature " << 
			first->name << " = " << (*first->pValue) <<
			" not set.");

	dc1394_feature_set_mode(p_camera_, first->code, DC1394_FEATURE_MODE_MANUAL);
#endif
      }
    }

    // white balance
    if (params_.whiteBalance0 == -1 ||
	params_.whiteBalance1 == -1) {
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
      dc1394_auto_on_off(handle_, p_camera_->node, FEATURE_WHITE_BALANCE, 1);
#else
      dc1394_feature_set_mode(p_camera_, DC1394_FEATURE_WHITE_BALANCE, DC1394_FEATURE_MODE_AUTO);
#endif
    }
    else if (params_.whiteBalance0 >= 0 &&
	     params_.whiteBalance1 >= 0) {
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
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
#else
      if (dc1394_feature_whitebalance_set_value(p_camera_,
						params_.whiteBalance0,
						params_.whiteBalance1) != DC1394_SUCCESS) {
	MIRO_LOG_OSTR(LL_WARNING,
		      "Feature WhiteBalance = " <<
		      params_.whiteBalance0 << ":" << params_.whiteBalance1 <<
		      " not set.");
      }
      dc1394_feature_set_mode(p_camera_, DC1394_FEATURE_WHITE_BALANCE, DC1394_FEATURE_MODE_MANUAL);
#endif
    }

    // set framerate
    int value = params_.framerate;
#if LIBDC1394_VERSION == 1 || LIBDC1394_VERSION == 2
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
#else
    if (value <= 1)	   
      frameRate_ = DC1394_FRAMERATE_1_875;
    else if (value <= 3)
      frameRate_ = DC1394_FRAMERATE_3_75;
    else if (value <= 7)
      frameRate_ = DC1394_FRAMERATE_7_5;
    else if (value <= 15)
      frameRate_ = DC1394_FRAMERATE_15;
    else
      frameRate_ = DC1394_FRAMERATE_30;
#endif

    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::initSettings() finished");
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

