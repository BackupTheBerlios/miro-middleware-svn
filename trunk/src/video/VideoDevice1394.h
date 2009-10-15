// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
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
#ifndef VIDEODEVICE1394_H
#define VIDEODEVICE1394_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "VideoDevice.h"
#include "idl/VideoControlS.h"

#include <libraw1394/raw1394.h>
#include <dc1394/control.h>
#include <string.h>

namespace Video
{

	//Class Describing everything about a camera feature
	struct Feat1394Desc
	{
		std::string name;
		CameraFeature miroCode;
		dc1394feature_t code;
		bool avail;
		AutoType autoModes;
		
		int xmlparam;

		AutoType setMode;
		int setValue;

		AutoType currMode;
		int currVal;
		int minVal;
		int maxVal;
	};


	//Index of Table corresponds to CameraFeature enum val
	//Thus you can quickly relate the 1394 feature to the miro feature
	struct FeatDescTable
	{

		#define NUM_FEATURES 15
public:
		int num_features;
		Feat1394Desc feature[NUM_FEATURES];

		FeatDescTable( Device1394Parameters &param )
		{

			num_features = NUM_FEATURES; 
			feature[0].code = DC1394_FEATURE_BRIGHTNESS;
			feature[1].code = DC1394_FEATURE_EXPOSURE;
			feature[2].code = DC1394_FEATURE_FOCUS;
			feature[3].code = DC1394_FEATURE_GAIN;
			feature[4].code = DC1394_FEATURE_GAMMA;
			feature[5].code = DC1394_FEATURE_HUE;
			feature[6].code = DC1394_FEATURE_IRIS;
			feature[7].code = DC1394_FEATURE_SATURATION;
			feature[8].code = DC1394_FEATURE_SHARPNESS;
			feature[9].code = DC1394_FEATURE_SHUTTER;
			feature[10].code = DC1394_FEATURE_TEMPERATURE;
			feature[11].code = DC1394_FEATURE_TRIGGER;
			feature[12].code = DC1394_FEATURE_ZOOM;
			feature[13].code = DC1394_FEATURE_WHITE_BALANCE;
			feature[14].code = DC1394_FEATURE_WHITE_BALANCE;;

			feature[0].miroCode = BRIGHTNESS;
			feature[1].miroCode = EXPOSURE;
			feature[2].miroCode = FOCUS;
			feature[3].miroCode = GAIN;
			feature[4].miroCode = GAMMA;
			feature[5].miroCode = HUE;
			feature[6].miroCode = IRIS;
			feature[7].miroCode = SATURATION;
			feature[8].miroCode = SHARPNESS;
			feature[9].miroCode = SHUTTER;
			feature[10].miroCode = TEMPERATURE;
			feature[11].miroCode = TRIGGER;
			feature[12].miroCode = ZOOM;
			feature[13].miroCode = WHITE_BALANCE_BLUE;
			feature[14].miroCode = WHITE_BALANCE_RED;

			feature[0].name = "BRIGHTNESS";
			feature[1].name = "EXPOSURE";
			feature[2].name = "FOCUS";
			feature[3].name = "GAIN";
			feature[4].name = "GAMMA";
			feature[5].name = "HUE";
			feature[6].name = "IRIS";
			feature[7].name = "SATURATION";
			feature[8].name = "SHARPNESS";
			feature[9].name = "SHUTTER";
			feature[10].name = "TEMPERATURE";
			feature[11].name = "TRIGGER";
			feature[12].name = "ZOOM";
			feature[13].name = "WHITE_BALANCE_BLUE";
			feature[14].name = "WHITE_BALANCE_RED";

			feature[0].xmlparam = param.brightness;
			feature[1].xmlparam = param.exposure;
			feature[2].xmlparam = param.focus;
			feature[3].xmlparam = param.gain;
			feature[4].xmlparam = param.gamma;
			feature[5].xmlparam = param.hue;
			feature[6].xmlparam = param.iris;
			feature[7].xmlparam = param.saturation;
			feature[8].xmlparam = param.sharpness;
			feature[9].xmlparam = param.shutter;
			feature[10].xmlparam = param.temperature;
			feature[11].xmlparam = param.trigger;
			feature[12].xmlparam = param.zoom;
			feature[13].xmlparam = param.whiteBalance0;
			feature[14].xmlparam = param.whiteBalance1;
		};

		friend std::ostream& operator << (std::ostream& os, const FeatDescTable* data);
	};


  //! Firewire camera support.
  class Device1394 : public Device, public virtual POA_Video::CameraControl
  {
    typedef Device Super;
    
  public:
    Device1394(const Miro::ImageFormatIDL& _inputFormat);
    virtual ~Device1394();
	
    FILTER_PARAMETERS_FACTORY(Device1394);

//     bool setFeatures(const FeatureSet & features);
//     bool getFeatures(FeatureSet & features) const;

    virtual void getFeature(CameraFeature feature, FeatureSet_out set)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ));
    virtual void setFeature(CameraFeature feature, const FeatureSet & set)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ));
    virtual void getFeatureDescription (FeatureSetVector_out features)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ));

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

    //! local helper class
    int valueOrNeg(FeatureSet _set);

    Device1394Parameters    params_;

  private:
	
	void fillFeatureTable();
	void paramToFeatureSet( int param, FeatureSet &set );
    //! Flag to indicate that the device is initialized.
    bool                    is_open_;
    //! Real device handle.
    raw1394handle_t         handle_;
    dc1394_t *dc1394;
    //! Actual camera features.
    dc1394featureset_t features_;
    //! Camera capture data structure.
    dc1394camera_t * p_camera_;
    //! Selected image format.
    dc1394video_mode_t      imageFormat_;
    //! Framerate
    dc1394framerate_t       frameRate_;

	 //!Internal structure storing all the camera data
	 FeatDescTable *myCamera;
  };
};
#endif // VIDEODEVICE1394_H

