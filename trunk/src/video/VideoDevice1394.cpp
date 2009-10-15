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

};

namespace Video
{

	struct FeatDescTable;
	std::ostream& operator<<(std::ostream& os, const FeatDescTable* data)
	{
		for( int i = 0; i < data->num_features; i++ )
		{
			os << "Feature: " << data->feature[i].name << " " 
				<< "Code: " << data->feature[i].code << " "
				<< "Avail: " << data->feature[i].avail << " "
				<< "AutoModes: " << data->feature[i].autoModes << " "
				<< "CurrMode: " << data->feature[i].currMode << " "
				<< "CurrVal: " << data->feature[i].currVal << " "
				<< "SetMode: " << data->feature[i].setMode << " "
				<< "SetVal: " << data->feature[i].setValue << " "
				<< "MinVal: " << data->feature[i].minVal << " "
				<< "MaxVal: " << data->feature[i].maxVal << std::endl;
		}
		return os;
	}

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

	 myCamera = new FeatDescTable( params_ ); 

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

    //Get Camera Features
    if( dc1394_feature_get_all(p_camera_, &features_) != DC1394_SUCCESS )
    {
		throw Miro::Exception("Device1394::initDevice: unable to get camera features");
    }

	 fillFeatureTable();

    if (Miro::Log::level() >= Miro::Log::LL_NOTICE)
    {
        dc1394_feature_print_all(&features_, stdout );
    }
  }

	void Device1394::fillFeatureTable()
	{
		int automodes;

		//Loop through all the camera features and populate the internal structure
		for( int i = 0; i < DC1394_FEATURE_NUM; i++ )
		{
			for( int j = 0; j < myCamera->num_features; j++ )
			{
				if( features_.feature[i].id == myCamera->feature[j].code )
				{

					if( features_.feature[i].available == false )
					{
						myCamera->feature[j].avail = false;
					}
					else
					{
						automodes = 0;
						myCamera->feature[j].avail = true;
						myCamera->feature[j].minVal =features_.feature[i].min;
						myCamera->feature[j].maxVal =features_.feature[i].max;
						myCamera->feature[j].currVal =features_.feature[i].value;
						for( int k = 0; k < features_.feature[i].modes.num; k++ )
						{
							if( features_.feature[i].modes.modes[k] == DC1394_FEATURE_MODE_AUTO )
							{
								automodes += AUTO;
							}

							if(  features_.feature[i].modes.modes[k] == DC1394_FEATURE_MODE_ONE_PUSH_AUTO )
							{
								automodes += AUTO_ONE_PUSH;
							}
						}

						switch( automodes )
						{
							case 0: myCamera->feature[j].autoModes = MANUAL; break;
							case 1: myCamera->feature[j].autoModes = AUTO; break;
							case 2: myCamera->feature[j].autoModes = AUTO_ONE_PUSH; break;
							case 3: myCamera->feature[j].autoModes = AUTO_BOTH;
						}

						switch( features_.feature[i].current_mode )
						{
							case DC1394_FEATURE_MODE_AUTO: myCamera->feature[j].currMode = AUTO; break;
							case DC1394_FEATURE_MODE_ONE_PUSH_AUTO: myCamera->feature[j].currMode = AUTO_ONE_PUSH; break;
							case DC1394_FEATURE_MODE_MANUAL: myCamera->feature[j].currMode = MANUAL; break;
						}
						
					}
					
					if( features_.feature[i].id == DC1394_FEATURE_WHITE_BALANCE )
					{
						if( j == WHITE_BALANCE_BLUE )
							myCamera->feature[j].currVal = features_.feature[i].BU_value;
						else if( j == WHITE_BALANCE_RED )
							myCamera->feature[j].currVal = features_.feature[i].RV_value;
					}
					else
					{
						break;
					}

				}
			}
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

	 FeatureSet set;
	 for( int i = 0; i < myCamera->num_features; i++ )
	 {
		paramToFeatureSet( myCamera->feature[i].xmlparam, set );
	   setFeature( myCamera->feature[i].miroCode, set);
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

	void Device1394::paramToFeatureSet( int param, FeatureSet &set )
	{
		if( param == -1 )
		{
			set.autoMode = AUTO;
			set.value = 0;
		}
		else if( param == -2 )
		{
			set.autoMode = AUTO_ONE_PUSH;
			set.value = 0;
		}
		else if( param >= 0 )
		{
			set.autoMode = MANUAL;
			set.value = param;
		}
	}

  void
  Device1394::getFeature(CameraFeature feature, FeatureSet_out set)
    ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {
	

	uint32_t tmp;
	uint32_t value;
	dc1394feature_mode_t mode;
	switch( myCamera->feature[feature].miroCode )
	{
		case WHITE_BALANCE_BLUE: dc1394_feature_whitebalance_get_value( p_camera_, &value, &tmp); break;
		case WHITE_BALANCE_RED: dc1394_feature_whitebalance_get_value( p_camera_, &tmp, &value); break;
		default:
		{
			dc1394_feature_get_value( p_camera_, myCamera->feature[feature].code, &value );
			break;
		}
		
	}

	dc1394_feature_get_mode( p_camera_, myCamera->feature[feature].code, &mode ); 
	
	switch( mode )
	{
		case DC1394_FEATURE_MODE_AUTO: set.autoMode = AUTO; break;
		case DC1394_FEATURE_MODE_MANUAL: set.autoMode = MANUAL; break;
		case DC1394_FEATURE_MODE_ONE_PUSH_AUTO: set.autoMode = AUTO_ONE_PUSH; break;
		default: cout << "UNKNOWN MODE " << endl; break;
	}
	
	set.value = (int) value;
	
	myCamera->feature[feature].currMode = set.autoMode;
	myCamera->feature[feature].currVal = set.value;

  }


  int
  Device1394::valueOrNeg(FeatureSet _set)
  {
    if (_set.autoMode == AUTO )
      return -1;
    else if (_set.autoMode == AUTO_ONE_PUSH )
      return -2;
	
    return (int)_set.value;
  }
   

  void
  Device1394::setFeature(CameraFeature feature, const FeatureSet & set)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {

	//Check if feature is avail, value falls between min/max, and autoMode is avail
	if( myCamera->feature[feature].avail == false )
	{
		cout << "Feature " << myCamera->feature[feature].name << " is unavailable for this camera" << endl;
		return;
	}

	if( myCamera->feature[feature].autoModes < set.autoMode )
	{
		cout << "Feature " << myCamera->feature[feature].name << " does not support automode " << set.autoMode << endl;
		return;
	}

	if( set.autoMode == MANUAL )
	{
		if( set.value < myCamera->feature[feature].minVal || set.value > myCamera->feature[feature].maxVal )
		{
			cout << "Value " << set.value << " falls outside the range min/max " << myCamera->feature[feature].minVal << "/" << myCamera->feature[feature].maxVal << " for feature " << myCamera->feature[feature].name << endl;
			return;
		}
	}

	myCamera->feature[feature].setValue = set.value;
	myCamera->feature[feature].setMode = set.autoMode;

	
	if( myCamera->feature[feature].avail == true )
	{
	switch( feature )
	{
		case WHITE_BALANCE_BLUE: 
		case WHITE_BALANCE_RED:
		{
	    	if (myCamera->feature[WHITE_BALANCE_BLUE].setMode == AUTO || myCamera->feature[WHITE_BALANCE_BLUE].setMode == AUTO ) 
			{
      		dc1394_feature_set_mode(p_camera_, DC1394_FEATURE_WHITE_BALANCE, DC1394_FEATURE_MODE_AUTO);
    		}
	    	else if (myCamera->feature[WHITE_BALANCE_BLUE].setValue == AUTO_ONE_PUSH || myCamera->feature[WHITE_BALANCE_BLUE].setValue == AUTO_ONE_PUSH ) 
	 		{
				dc1394_feature_set_mode(p_camera_, DC1394_FEATURE_WHITE_BALANCE, DC1394_FEATURE_MODE_ONE_PUSH_AUTO);
	 		}
    		else if (myCamera->feature[WHITE_BALANCE_BLUE].setValue >= 0 && myCamera->feature[WHITE_BALANCE_BLUE].setValue >= 0 )  
			{
      		if (dc1394_feature_whitebalance_set_value(p_camera_, myCamera->feature[WHITE_BALANCE_BLUE].setValue, myCamera->feature[WHITE_BALANCE_RED].setValue) != DC1394_SUCCESS) 
				{
					MIRO_LOG_OSTR(LL_WARNING,  "Feature WhiteBalance = " << params_.whiteBalance0 << ":" << params_.whiteBalance1 << " not set.");
      		}
				else
				{
						myCamera->feature[feature].currVal = myCamera->feature[feature].setValue;
				}
      		dc1394_feature_set_mode(p_camera_, DC1394_FEATURE_WHITE_BALANCE, DC1394_FEATURE_MODE_MANUAL);
    		}
			break;
		}
		default:
		{
			if( myCamera->feature[feature].setMode == AUTO )
			{
				 dc1394_feature_set_mode(p_camera_, myCamera->feature[feature].code, DC1394_FEATURE_MODE_AUTO);
			}
			else if( myCamera->feature[feature].setMode == AUTO_ONE_PUSH )
			{
		 		dc1394_feature_set_mode(p_camera_, myCamera->feature[feature].code, DC1394_FEATURE_MODE_ONE_PUSH_AUTO);
			}
			else 
			{
				if (dc1394_feature_set_value(p_camera_, myCamera->feature[feature].code, myCamera->feature[feature].setValue) != DC1394_SUCCESS) 
				{
		  			MIRO_LOG_OSTR(LL_WARNING, "Feature " << myCamera->feature[feature].name << " = " << myCamera->feature[feature].setValue << " not set.");
				}
				else
				{
					myCamera->feature[feature].currVal = myCamera->feature[feature].setValue;
				}
				dc1394_feature_set_mode(p_camera_, myCamera->feature[feature].code, DC1394_FEATURE_MODE_MANUAL);
			}
			break;
		}
	}
	}
	else
	{
		cout << "Feature " << myCamera->feature[feature].name << " Can't be set because it is not available on this camera" << endl;
	}
  }


  void
  Device1394::getFeatureDescription (FeatureSetVector_out features)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {

    if( dc1394_feature_get_all(p_camera_, &features_) != DC1394_SUCCESS )
    {
		throw Miro::Exception("Device1394::initDevice: unable to get camera features");
    }
	fillFeatureTable();

   FeatureSetVector f;
	f.length(0);
	FeatureDescription desc;

	for( int i = 0; i < myCamera->num_features; i++ )
	{
		//Only return available features
		if( myCamera->feature[i].avail == true )
		{
			desc.feature = myCamera->feature[i].miroCode;
			desc.hasAutoMode  = myCamera->feature[i].autoModes;
			desc.value = myCamera->feature[i].currVal;
			desc.minValue = myCamera->feature[i].minVal;
			desc.maxValue = myCamera->feature[i].maxVal;
			f.length( f.length() + 1 );
			f[f.length()-1] = desc;
		}
	}

	 features = new FeatureSetVector(f);

  }
};

