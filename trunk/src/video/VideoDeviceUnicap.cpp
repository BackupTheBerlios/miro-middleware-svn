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
#include "VideoDeviceUnicap.h"
#include "BufferManagerUnicap.h"

#include <miro/Exception.h>
#include <miro/Log.h>
#include <miro/Server.h>

#include <ace/OS.h>
#include <iostream>

using std::cout;
using std::endl;



char * data = new char[640 * 480 * 2];



namespace
{
  #define FOURCC(a,b,c,d) (unsigned int)((((unsigned int)d)<<24)+(((unsigned int)c)<<16)+(((unsigned int)b)<<8)+a)

  struct ModeUnicap
  {
    Miro::ImageFormatIDL format;
    int width;
    int height;
    int fourcc;
    int bpp;
  };
  static ModeUnicap modeUnicap[] = {
    { {640, 480, Miro::YUV_422}, 640, 480, FOURCC('U','Y','V','Y'), 16},
    { {640, 480, Miro::GREY_8},  640, 480, FOURCC('Y','8','0','0'), 8}
  };


  struct MenuItemsUnicap
  {
    char const * value;
    char const * unicap;
  };
  static MenuItemsUnicap modeItemsStrobeModeUnicap[] = {
    {"ConstantLow",   "constant low"},
    {"ConstantHigh",  "constant high"},
    {"FixedDuration", "fixed duration"},
    {"Exposure",      "exposure"}
  };
  static MenuItemsUnicap modeItemsStrobePolarityUnicap[] = {
    {"ActiveLow",   "active low"},
    {"ActiveHigh",  "active high"}
  };

  struct UnicapValueControlValue
  {
    char * const unicapName;
    Video::CameraFeature feature;
    bool autoMode;
  };
  static UnicapValueControlValue unicapValueControlValue[] = {
    {"brightness",      Video::BRIGHTNESS, false},
    {"strobe_duration", Video::STROBE_DURATION, false},
    {"strobe_delay",    Video::STROBE_DELAY, false},
    {"auto_exposure",   Video::EXPOSURE, false},
    {"white_balance_u", Video::WHITE_BALANCE_BLUE, true},
    {"white_balance_v", Video::WHITE_BALANCE_RED, false},
    {"hue",             Video::HUE, false},
    {"saturation",      Video::SATURATION, false},
    {"gamma",           Video::GAMMA, false},
    {"timeout",         Video::TIMEOUT, false},
    {"shutter",         Video::SHUTTER, true},
    {"gain",            Video::GAIN, true}
  };
};


namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(DeviceUnicap);


  DeviceUnicap::DeviceUnicap(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat)
  {
    MIRO_LOG_CTOR("Video::DeviceUnicap.");
  }
    

  DeviceUnicap::~DeviceUnicap()
  {
    MIRO_LOG_DTOR("Video::DeviceUnicap.");
  }


  BufferManager * 
  DeviceUnicap::bufferManagerInstance() const 
  {
    return new BufferManagerUnicap(this, params_.buffers, format_.buffer_size, handle_, format_);
  }


  void
  DeviceUnicap::init(Miro::Server& _server, FilterParameters const * _params)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "DeviceUnicap::init()");

    if (_params)
      params_ = *(dynamic_cast<DeviceUnicapParameters const *>(_params));

    // SETUP DEVICE

    // Get the first device found by the unicap library
    if( !SUCCESS( unicap_enumerate_devices( NULL, &device_, 0 ) ) )
      throw Miro::Exception("DeviceUnicap: No device found");

    // Acquire a handle to this device
    if( !SUCCESS( unicap_open( &handle_, &device_ ) ) )
      throw Miro::Exception("DeviceUnicap: Failed to open device: "+ std::string(device_.identifier) );

    // SETUP FORMAT

    // search the list of format presets
    unsigned int requested;
    for (requested=0; requested<(sizeof(modeUnicap)/sizeof(modeUnicap[0])); ++requested)
    {
      if (inputFormat_.width  == modeUnicap[requested].format.width &&
	  inputFormat_.height == modeUnicap[requested].format.height &&
	  inputFormat_.palette == modeUnicap[requested].format.palette ) {
	break;
      }
    }

    if (requested == (sizeof(modeUnicap)/sizeof(modeUnicap[0])))
      throw Miro::Exception("DeviceUnicap: Unsupported image format requested.");

    // now compare all available formats of the camera to the requested one (its done this
    // way, cause i dont know the exact meaning of all of the fields used in the camera
    // format definition)
    int count = 0;
    unicap_status_t status = STATUS_SUCCESS;
    while (status != STATUS_NO_MATCH) {
      status = unicap_enumerate_formats( handle_, NULL, &format_, count );
      if (format_.fourcc == modeUnicap[requested].fourcc &&
	  format_.bpp == modeUnicap[requested].bpp) {
	format_.size.width = modeUnicap[requested].width;
	format_.size.height = modeUnicap[requested].height;
	break;
      }
      count++;
    }
    if (status == STATUS_NO_MATCH)
      throw Miro::Exception("DeviceUnicap: Failed to find requested format in camera offers: "+ std::string(device_.identifier));

    // Set this video format
    format_.buffer_type = UNICAP_BUFFER_TYPE_SYSTEM; // need when using system buffer
    if( !SUCCESS( unicap_set_format( handle_, &format_ ) ) )
      throw Miro::Exception("DeviceUnicap: Failed to set video format");

    // SETUP PARAMETERS

    // collect all possible parameters
    propertiesCount_ = 0;
    status = STATUS_SUCCESS;
    while (status != STATUS_NO_MATCH) {
      status = unicap_enumerate_properties( handle_, NULL, &properties_[propertiesCount_], propertiesCount_ );
      propertiesCount_++;
      if (propertiesCount_ >= MAX_PROPERTIES_)
	throw Miro::Exception("DeviceUnicap: More properties available then expected");
    }
    // set them to new values
    setupParameters();

    CameraControl_ptr controller = this->_this();
    _server.addToNameService(controller, "CameraControlUnicap");

    Super::init(_server, _params);
  }


  void
  DeviceUnicap::fini()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "DeviceUnicap::fini()");

    // Stop the device
    if( !SUCCESS( unicap_stop_capture( handle_ ) ) )
      throw Miro::Exception("DeviceUnicap: Failed to stop capture on device: " + std::string(device_.identifier));

    // Close the device. This invalidates the handle
    if( !SUCCESS( unicap_close( handle_ ) ) )
      throw Miro::Exception("DeviceUnicap: Failed to close the device: " + std::string(device_.identifier));
  }


  void
  DeviceUnicap::setupParameters()
  {
    setupParametersMenu("strobe_mode",        params_.strobeMode);
    setupParametersMenu("strobe_polarity",    params_.strobePolarity);

    setupParametersRange("brightness",      params_.brightness);
    setupParametersRange("strobe_duration", params_.strobeDuration);
    setupParametersRange("strobe_delay",    params_.strobeDelay);
    setupParametersRange("auto_exposure",   params_.exposure);
    setupParametersRange("white_balance_u", params_.whiteBalanceU);
    setupParametersRange("white_balance_v", params_.whiteBalanceV);
    setupParametersRange("hue",             params_.hue);
    setupParametersRange("saturation",      params_.saturation);
    setupParametersRange("gamma",           params_.gamma);
    setupParametersRange("timeout",         params_.timeout);

    if (params_.shutter >= 0)
      setupParametersRange("shutter", params_.shutter);
    else {
      int i = getProperty("shutter");
      properties_[i].flags = UNICAP_FLAGS_AUTO;
      if( !SUCCESS( unicap_set_property( handle_, &properties_[i] ) ) )
	throw Miro::Exception("DeviceUnicap:setParam - Failed to set flag for property: shutter");
    }


    if (params_.gain >= 0)
      setupParametersRange("gain", params_.gain);
    else {
      int i = getProperty("gain");
      properties_[i].flags = UNICAP_FLAGS_AUTO;
      if( !SUCCESS( unicap_set_property( handle_, &properties_[i] ) ) )
	throw Miro::Exception("DeviceUnicap:setParam - Failed to set flag for property: gain");
    }


    int i;
    i = getProperty("white_balance_mode");
    if (params_.whiteBalanceMode.value() == "ON_OFF")
      properties_[i].flags = UNICAP_FLAGS_ON_OFF;
    else if (params_.whiteBalanceMode.value() == "AUTO")
      properties_[i].flags = UNICAP_FLAGS_AUTO;
    else
      throw Miro::Exception("DeviceUnicap:setParam - No value " + params_.whiteBalanceMode.value() + " for white balance mode");
    if( !SUCCESS( unicap_set_property( handle_, &properties_[i] ) ) )
      throw Miro::Exception("DeviceUnicap:setParam - Failed to set property: white balance mode");


    i = getProperty("frame rate");
    int j;
    for (j=0; j<properties_[i].value_list.value_count; ++j) {
      if (params_.framerate == properties_[i].value_list.values[j]) {
	properties_[i].value = properties_[i].value_list.values[j];
	break;
      }
    }

    if (j >= properties_[i].value_list.value_count)
      throw Miro::Exception("DeviceUnicap:setParam -"
			    "Failed to find the given value in the list of possible values for \"frame rate\"");
    if( !SUCCESS( unicap_set_property( handle_, &properties_[i] ) ) )
      throw Miro::Exception("DeviceUnicap:setParam - Failed to set property: frame rate");


    // STILL TO IMPLEMENT

    // data properties accept an arbitrary amount of data pointed to in the property_data
    // field. property_data_size should be set to the size of the data block in
    // bytes. While all properties may accept optional data in the property_data field,
    // data properties will not evaluate the value or menu_item field.
    //  i = getProperty("register");
    //  i = getProperty("gpio");
  }


  int
  DeviceUnicap::getProperty(const std::string _property)  const
  {
    int i;
    for (i=0; i<propertiesCount_; ++i)
      if (std::string(properties_[i].identifier) == _property)
	return i;
    throw Miro::Exception("DeviceUnicap:setParam - Property not found: " + _property);
    return -1;
  }


  void 
  DeviceUnicap::setupParametersRange(std::string _property, double _value)
  {
    int i = getProperty(_property);

    if (!SUCCESS(unicap_get_property( handle_, &properties_[i] )))
      throw Miro::Exception("DeviceUnicap:setParam - Failed to inquire property: " + _property);

    if (properties_[i].type != UNICAP_PROPERTY_TYPE_RANGE )
      throw Miro::Exception("DeviceUnicap:setParam - Property is not of type range: " + _property);

    if (_value < properties_[i].range.min ||
	_value > properties_[i].range.max) {
      std::cerr << "Designated value: " << _value << std::endl
		<< "Possible minimal value: " << properties_[i].range.min << std::endl
		<< "Possible maximal value: " << properties_[i].range.max << std::endl;
      throw Miro::Exception("DeviceUnicap:setParam - Property is out of range: " + _property);
    }
    
    properties_[i].value = _value;
    properties_[i].flags = UNICAP_FLAGS_MANUAL;
    if( !SUCCESS( unicap_set_property( handle_, &properties_[i] ) ) )
      throw Miro::Exception("DeviceUnicap:setParam - Failed to set property: " + _property);
  }


  void
  DeviceUnicap::setupParametersMenu(std::string _property, Miro::Enumeration _enum)
  {
    int i = getProperty(_property);

    if (!SUCCESS(unicap_get_property( handle_, &properties_[i] )))
      throw Miro::Exception("DeviceUnicap:setParam - Failed to inquire property: " + _property);

    if (properties_[i].type != UNICAP_PROPERTY_TYPE_MENU )
      throw Miro::Exception("DeviceUnicap:setParam - Property is not of type menu: " + _property);

    if (_property == "strobe_mode") {
      for (int j=0; j<(sizeof(modeItemsStrobeModeUnicap)/sizeof(modeItemsStrobeModeUnicap[0])); ++j)
	if (std::string(modeItemsStrobeModeUnicap[j].value) == _enum.value()) {
	  strcpy( properties_[i].menu_item, modeItemsStrobeModeUnicap[j].unicap );
	  break;
	}
    }
    else if (_property == "strobe_polarity") {
      for (int j=0; j<(sizeof(modeItemsStrobePolarityUnicap)/sizeof(modeItemsStrobePolarityUnicap[0])); ++j)
	if (std::string(modeItemsStrobePolarityUnicap[j].value) == _enum.value()) {
	  strcpy( properties_[i].menu_item, modeItemsStrobePolarityUnicap[j].unicap );
	  break;
	}
    }
    else 
      throw Miro::Exception("DeviceUnicap:setParam - No value table for this property: " + _property);

    if ( !SUCCESS( unicap_set_property( handle_, &properties_[i] ) ) )
      throw Miro::Exception("DeviceUnicap:setParam - Failed to set property: " + _property);
  }


  void
  DeviceUnicap::getFeature(CameraFeature feature, FeatureSet_out set)
    ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {
    for (unsigned int i=0; i<sizeof(unicapValueControlValue)/sizeof(unicapValueControlValue[0]); ++i) {
      if (unicapValueControlValue[i].feature == feature) {
	int prop = getProperty(unicapValueControlValue[i].unicapName);	
	set.value = properties_[prop].value;
	if (properties_[prop].flags & UNICAP_FLAGS_MANUAL)
	  set.autoMode = false;
	else
	  set.autoMode = true;

	break;
      }
    }
  }


  void
  DeviceUnicap::setFeature(CameraFeature feature, const FeatureSet & set)
    ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {
    for (unsigned int i=0; i<sizeof(unicapValueControlValue)/sizeof(unicapValueControlValue[0]); ++i) {
      if (unicapValueControlValue[i].feature == feature) {
	int prop = getProperty(unicapValueControlValue[i].unicapName);
	if (set.autoMode) {
	  properties_[prop].flags = UNICAP_FLAGS_AUTO;
	} else {
	  properties_[prop].value = set.value;
	  properties_[prop].flags = UNICAP_FLAGS_MANUAL;
	}

	if( !SUCCESS( unicap_set_property( handle_, &properties_[prop] ) ) )
	  throw Miro::Exception("DeviceUnicap:setParam - Failed to set property");
	break;
      }
    }
  }


  void
  DeviceUnicap::getFeatureDescription (FeatureSetVector_out features)
    ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ))
  {
    FeatureSetVector f;
    f.length(sizeof(unicapValueControlValue)/sizeof(unicapValueControlValue[0]));

    for (unsigned int i=0; i<sizeof(unicapValueControlValue)/sizeof(unicapValueControlValue[0]); ++i) {
      int prop = getProperty(unicapValueControlValue[i].unicapName);
      FeatureDescription desc = {unicapValueControlValue[i].feature,
				 unicapValueControlValue[i].autoMode,
				 properties_[prop].value,
				 properties_[prop].range.min,
				 properties_[prop].range.max};
      f[i] = desc;
    }

    features = new FeatureSetVector(f);
  }
};

