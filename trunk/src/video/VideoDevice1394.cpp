// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "VideoDevice1394.h"
#include "BufferManager1394.h"

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
  // mapping of miro formats to libdc1394 formats
  struct Mode1394
  {
    Miro::ImageFormatIDL format;
    dc1394video_mode_t mode;
  };

  static Mode1394 mode1394[] = {
    { {160, 120, Miro::YUV_24 }, DC1394_VIDEO_MODE_160x120_YUV444},
    { {320, 240, Miro::YUV_422}, DC1394_VIDEO_MODE_320x240_YUV422},
    { {640, 480, Miro::YUV_411}, DC1394_VIDEO_MODE_640x480_YUV411},
    { {640, 480, Miro::YUV_422}, DC1394_VIDEO_MODE_640x480_YUV422},
    { {640, 480, Miro::RGB_24 }, DC1394_VIDEO_MODE_640x480_RGB8},
    { {640, 480, Miro::GREY_8 }, DC1394_VIDEO_MODE_640x480_MONO8},
    { {640, 480, Miro::GREY_16}, DC1394_VIDEO_MODE_640x480_MONO16}
  };
	
  static Mode1394 mode11394[] = {
    { {800, 600, Miro::YUV_422}, DC1394_VIDEO_MODE_800x600_YUV422},
    { {1024, 768, Miro::YUV_422}, DC1394_VIDEO_MODE_1024x768_YUV422},
    { {800, 600, Miro::RGB_24 }, DC1394_VIDEO_MODE_800x600_RGB8},
    { {1024, 768, Miro::RGB_24 }, DC1394_VIDEO_MODE_1024x768_RGB8},
    { {800, 600, Miro::GREY_8}, DC1394_VIDEO_MODE_800x600_MONO8},
    { {1024, 768, Miro::GREY_8}, DC1394_VIDEO_MODE_1024x768_MONO8},
    { {800, 600, Miro::GREY_16}, DC1394_VIDEO_MODE_800x600_MONO16},
    { {1024, 768, Miro::GREY_16}, DC1394_VIDEO_MODE_1024x768_MONO16}
  };

  //---------------------------------------------------------------
  struct Feature1394
  {
    char const * name;
    int const * pValue;
    dc1394feature_t code;
    int (* set_function) (raw1394handle_t handle,
			  nodeid_t node,
			  unsigned int sharpness);
  };

  static Feature1394 feature1394[13] = {
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
    { "trigger",     NULL, DC1394_FEATURE_TRIGGER,     NULL },
    { "zoom",        NULL, DC1394_FEATURE_ZOOM,        NULL }

  };

  //---------------------------------------------------------------
  struct FeatureTable
  {
    static const unsigned int NUM_FEATURES1394 = 13;

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
      feature_[12].pValue = & (_params.zoom);
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
      p_camera_(NULL),
    frameRate_(DC1394_FRAMERATE_30)


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

    CameraControl_ptr controller = this->_this();
    _server.addToNameService(controller, "CameraControl1394");

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
      dc1394_video_set_transmission(p_camera_, DC1394_OFF);
      is_open_ = false;
    }

    cleanupDevice();
  }
    
  //---------------------------------------------------------------
  void
  Device1394::initDevice(int port)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::initDevice()");

    //Grab the list of cameras
    dc1394error_t err;
    dc1394 = dc1394_new();
    dc1394camera_list_t *camera_list;
    err=dc1394_camera_enumerate(dc1394,&camera_list);
    if( err != DC1394_SUCCESS )
    {	
        throw Miro::Exception("Device::intiDevice Failed to enumerate cameras");
    }

    if (camera_list->num == 0) {
        throw Miro::Exception("Device1394::initDevice no cameras found");
    }


    MIRO_DBG_OSTR(VIDEO, LL_DEBUG, "Device1394::initDevice: cameracount = " << camera_list->num);


    p_camera_=dc1394_camera_new(dc1394,camera_list->ids[params_.camera].guid);

    if( !p_camera_ )
    {
	throw Miro::Exception("Device1394::initDevice Failed to initialize camera");
    }

    dc1394_camera_free_list ( camera_list );

    //Ensure transmission is stopped
//    dc1394_video_set_transmission(p_camera_, DC1394_OFF);

    //Get Camera Features
    if( dc1394_feature_get_all(p_camera_, &features_) != DC1394_SUCCESS )
    {
	throw Miro::Exception("Device1394::initDevice: unable to get camera features");
    }
    if (Miro::Log::level() >= Miro::Log::LL_NOTICE)
    {
        dc1394_feature_print_all(&features_, stdout );
    }
  }

  //---------------------------------------------------------------
  void 
  Device1394::cleanupDevice()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::cleanupDevice()");

    if (p_camera_)
      dc1394_camera_free(p_camera_);  
  }

  //---------------------------------------------------------------
  void
  Device1394::setImageFormat()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::setImageFormat()");

    unsigned int i;
	if( params_.videoMode == 0 )
	{
        for (i = 0; i < (sizeof(mode1394)/sizeof(mode1394[0])); ++i) 
		{
      	if (inputFormat_.width  == mode1394[i].format.width &&
	  	inputFormat_.height == mode1394[i].format.height &&
	  	inputFormat_.palette == mode1394[i].format.palette ) {
		imageFormat_ = mode1394[i].mode;
		break;
      	}
    }
	
    if (i == (sizeof(mode1394)/sizeof(mode1394[0])))
      throw Miro::Exception("Device1394::setImageFormat - Unsupported image format requested.");
	
	}
	else if ( params_.videoMode == 1 )
	{
    	for (i = 0; i < (sizeof(mode11394)/sizeof(mode11394[0])); ++i) 
		{
      	if (inputFormat_.width  == mode11394[i].format.width &&
	  	inputFormat_.height == mode11394[i].format.height &&
	  	inputFormat_.palette == mode11394[i].format.palette ) {
		imageFormat_ = mode11394[i].mode;
		break;
      	}
    }
	
    if (i == (sizeof(mode11394)/sizeof(mode11394[0])))
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

    dc1394speed_t speed;
    if (dc1394_video_get_iso_speed(p_camera_, &speed) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394:initDevice: unable to get iso speed number");
    MIRO_DBG_OSTR(VIDEO, LL_DEBUG, "ISO speed: " << speed);

    //Device strings are done automatically now
    if (dc1394_video_set_iso_speed(p_camera_, DC1394_ISO_SPEED_400) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::initCapture(): unable to set iso speed");
    if (dc1394_video_set_mode(p_camera_, imageFormat_) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::initCapture(): unable to set video mode");
    if (dc1394_video_set_framerate(p_camera_, frameRate_) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::initCapture(): unable to set framerate");
    if (dc1394_capture_setup(p_camera_, NUM_BUFFERS, DC1394_CAPTURE_FLAGS_DEFAULT) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::initCapture: unable to setup camera");

    if (dc1394_video_set_transmission(p_camera_,  DC1394_ON) != DC1394_SUCCESS)
      throw Miro::Exception("Device1394::handleConnect: unable to start camera iso transmission");

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
	dc1394_feature_set_mode(p_camera_, first->code, DC1394_FEATURE_MODE_AUTO);
      }
      else if (*(first->pValue) >= 0) {
	if (dc1394_feature_set_value(p_camera_, first->code, (*first->pValue)) != DC1394_SUCCESS)
	  MIRO_LOG_OSTR(LL_WARNING,
			"Feature " << 
			first->name << " = " << (*first->pValue) <<
			" not set.");

	dc1394_feature_set_mode(p_camera_, first->code, DC1394_FEATURE_MODE_MANUAL);
      }
    }

    // white balance
    if (params_.whiteBalance0 == -1 ||
	params_.whiteBalance1 == -1) {
      dc1394_feature_set_mode(p_camera_, DC1394_FEATURE_WHITE_BALANCE, DC1394_FEATURE_MODE_AUTO);
    }
    else if (params_.whiteBalance0 >= 0 &&
	     params_.whiteBalance1 >= 0) {
      if (dc1394_feature_whitebalance_set_value(p_camera_,
						params_.whiteBalance0,
						params_.whiteBalance1) != DC1394_SUCCESS) {
	MIRO_LOG_OSTR(LL_WARNING,
		      "Feature WhiteBalance = " <<
		      params_.whiteBalance0 << ":" << params_.whiteBalance1 <<
		      " not set.");
      }
      dc1394_feature_set_mode(p_camera_, DC1394_FEATURE_WHITE_BALANCE, DC1394_FEATURE_MODE_MANUAL);
    }

    // set framerate
    int value = params_.framerate;
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

    MIRO_DBG(VIDEO, LL_DEBUG, "Device1394::initSettings() finished");
  }


  void
  Device1394::getFeature(CameraFeature feature, FeatureSet_out set)
    ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {
    switch (feature) {
    case BRIGHTNESS:         params_.brightness < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.brightness; break;
    case EXPOSURE:           params_.exposure < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.exposure; break;
    case FOCUS:              params_.focus < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.focus; break;
    case GAIN:               params_.gain < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.gain; break;
    case GAMMA:              params_.gamma < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.gamma; break;
    case HUE:                params_.hue < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.hue; break;
    case IRIS:               params_.iris < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.iris; break;
    case SATURATION:         params_.saturation < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.saturation; break;
    case SHARPNESS:          params_.sharpness < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.sharpness; break;
    case SHUTTER:            params_.shutter < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.shutter; break;
    case TEMPERATURE:        params_.temperature < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.temperature; break;
    case TRIGGER:            params_.trigger < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.trigger; break;
    case WHITE_BALANCE_BLUE: params_.whiteBalance0 < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.whiteBalance0; break;
    case WHITE_BALANCE_RED:  params_.whiteBalance1 < 0 ? set.autoMode=true : set.autoMode=false; set.value = params_.whiteBalance1; break;
    case WHITE_BALANCE_SPEED:
    case WHITE_BALANCE_DELAY:
    case CONTRAST:
    case TIMEOUT:
    case STROBE_DURATION:
    case STROBE_DELAY:
    case BACKLIGHT_COMPENSATION:
    case FLICKERLESS_MODE:
    case DYNAMIC_NOISE_REDUCTION:
    case ZOOM:		    set.value = params_.zoom; break;
    case COMPRESSION:
      std::cout << "camera doesn't support this parameter" << std::endl;
    }
  }


  int
  Device1394::valueOrNeg(FeatureSet _set)
  {
    if (_set.autoMode)
      return -1;
    return (int)_set.value;
  }
   

  void
  Device1394::setFeature(CameraFeature feature, const FeatureSet & set)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {
    switch (feature) {
    case BRIGHTNESS:         params_.brightness = valueOrNeg(set); break;
    case EXPOSURE:           params_.exposure = valueOrNeg(set); break;
    case FOCUS:              params_.focus = valueOrNeg(set); break;
    case GAIN:               params_.gain = valueOrNeg(set); break;
    case GAMMA:              params_.gamma = valueOrNeg(set); break;
    case HUE:                params_.hue = valueOrNeg(set); break;
    case IRIS:               params_.iris = valueOrNeg(set); break;
    case SATURATION:         params_.saturation = valueOrNeg(set); break;
    case SHARPNESS:          params_.sharpness = valueOrNeg(set); break;
    case SHUTTER:            params_.shutter = valueOrNeg(set); break;
    case TEMPERATURE:        params_.temperature = valueOrNeg(set); break;
    case TRIGGER:            params_.trigger = valueOrNeg(set); break;
    case WHITE_BALANCE_BLUE: params_.whiteBalance0 = valueOrNeg(set); break;
    case WHITE_BALANCE_RED:  params_.whiteBalance1 = valueOrNeg(set); break;
    case WHITE_BALANCE_SPEED:
    case WHITE_BALANCE_DELAY:
    case CONTRAST:
    case TIMEOUT:
    case STROBE_DURATION:
    case STROBE_DELAY:
    case BACKLIGHT_COMPENSATION:
    case FLICKERLESS_MODE:
    case DYNAMIC_NOISE_REDUCTION:
    case ZOOM:		    params_.zoom = valueOrNeg(set);break;
    case COMPRESSION:
      std::cout << "camera doesn't support this parameter" << std::endl;
    }

    initSettings();
  }


  void
  Device1394::getFeatureDescription (FeatureSetVector_out features)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {
    FeatureSetVector f;
	f.length(0);
	FeatureDescription desc;
	bool valid = false;
	for( int i = 0; i < DC1394_FEATURE_NUM; i++ )
	{
		valid = false;
		switch( features_.feature[i].id )
		{
			case DC1394_FEATURE_BRIGHTNESS: valid = true; desc.feature = BRIGHTNESS; break;
			case DC1394_FEATURE_EXPOSURE: valid = true; desc.feature = EXPOSURE; break;
			case DC1394_FEATURE_FOCUS: valid = true; desc.feature = FOCUS; break;
			case DC1394_FEATURE_GAIN: valid = true; desc.feature = GAIN; break;
			case DC1394_FEATURE_GAMMA: valid = true; desc.feature = GAMMA; break;
			case DC1394_FEATURE_HUE: valid = true; desc.feature = HUE; break;
			case DC1394_FEATURE_IRIS: valid = true; desc.feature = IRIS; break;
			case DC1394_FEATURE_SATURATION: valid = true; desc.feature = SATURATION; break;
			case DC1394_FEATURE_SHARPNESS: valid = true; desc.feature = SHARPNESS; break;
			case DC1394_FEATURE_SHUTTER: valid = true; desc.feature = SHUTTER; break;
			case DC1394_FEATURE_TEMPERATURE: valid = true; desc.feature = TEMPERATURE; break;
			case DC1394_FEATURE_TRIGGER: valid = true; desc.feature = TRIGGER; break;
			case DC1394_FEATURE_WHITE_BALANCE: valid = true; desc.feature = WHITE_BALANCE_BLUE; break;
			case DC1394_FEATURE_ZOOM: valid = true; desc.feature = ZOOM; break;
		}

		if( features_.feature[i].available == false )
		{
			valid = false;
		}

		if( valid == true )
		{

			desc.hasAutoMode = false;
			for( int j = 0; j < features_.feature[i].modes.num; j++)
			{
				if( features_.feature[i].modes.modes[j] == DC1394_FEATURE_MODE_AUTO )
				{
					desc.hasAutoMode = true;
					break;
				}
			}

			desc.minValue = features_.feature[i].min;
			desc.maxValue = features_.feature[i].max;
			desc.value = features_.feature[i].value;
			f.length( f.length() + 1 );
			f[f.length()-1] = desc;

			if( desc.feature == WHITE_BALANCE_BLUE )
			{
				desc.feature = WHITE_BALANCE_RED;
				f.length( f.length() + 1 );
				f[f.length()-1] = desc;
				
			}
		}
	}

	 features = new FeatureSetVector(f);

  }
};

