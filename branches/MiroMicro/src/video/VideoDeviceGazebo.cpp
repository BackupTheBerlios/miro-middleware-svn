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
// $Id: VideoDeviceGazebo.cpp 1911 2008-08-16 00:07:15Z bs $
//
#include "VideoDeviceGazebo.h"
#include "BufferManager.h"
#include <miro/VideoHelper.h>

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(DeviceGazebo);


  DeviceGazebo::DeviceGazebo(const Miro::ImageFormatIDL& _inputFormat) :
   Super(_inputFormat),
	serverId_( 0 ),
	clientId_( GZ_CLIENT_ID_USER_FIRST )
  {
    MIRO_LOG(LL_NOTICE,"Create DeviceGazebo");
    interfaceAllowed_ = true;
	 imagebytes = getImageSize( _inputFormat );
  }

    
  DeviceGazebo::~DeviceGazebo()
  {
  }
	

  void DeviceGazebo::process()
  {
		bool success = true;
		gazebo::CameraIface *iface = camera_;

		if ( !iface )
		{
			std::cerr
				<< "[VideoDeviceGazebo::" << __FUNCTION__ << "] "
				<< "Camera interface has not been opened, exiting."
				<< std::endl;
			exit( 1 );
		}
		else
		{

			if ( !iface->Lock( 1 ) )
			{
				success = false;
			}
			else
			{
		
    unsigned char * outBuffer = outputBuffer();
    memcpy(outBuffer, iface->data->image, imagebytes );

    bufferManager_->bufferTimeStamp(outputBufferIndex_, ACE_OS::gettimeofday());

				if ( !iface->Unlock() )
				{
					success = false;
				}
			}
		}						
  }


  void DeviceGazebo::init(Miro::Server& _server, FilterParameters const * _params)
  {
    Super::init(_server, _params);

    MIRO_LOG(LL_NOTICE,"Initializing DeviceGazebo");

    DeviceGazeboParameters const * params = dynamic_cast<DeviceGazeboParameters const *>(_params);
    assert(params != NULL);

    MIRO_LOG(LL_NOTICE, "Video::DeviceGazebo: Connecting to Gazebo");
	 
  	try
	{
		client_ = new gazebo::Client();
   	client_->ConnectWait( serverId_, clientId_ );
   }
   catch ( std::string& e )
   {
   	std::cerr
   		<< "[GazeboService::GazeboClient::" << __FUNCTION__ << "] "
   		<< "Dammit! Can't connect to the libgazebo server." << std::endl
   		<< e;
   	exit( 1 );
   }

	camera_ = new gazebo::CameraIface();
	camera_->Open( client_, params->cameraIface.c_str() );
  }
}
