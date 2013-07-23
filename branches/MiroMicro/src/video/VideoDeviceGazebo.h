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
// $Id: VideoDeviceGazebo.h 1812 2007-02-25 07:09:56Z hutz $
//
#ifndef VideoDeviceGazebo_h
#define VideoDeviceGazebo_h

#include <gazebo/gazebo.h>
#include "VideoDevice.h"

namespace Video
{

	/// Miro video device which connects to the player robot simulator
  class DeviceGazebo : public Device
  {
    typedef Device Super;

  public:
    DeviceGazebo(const Miro::ImageFormatIDL& _inputFormatParameters);
    virtual ~DeviceGazebo();
	    	    
    FILTER_PARAMETERS_FACTORY(DeviceGazebo);

  protected:
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void process();

private:
	int serverId_;
	int clientId_;
	gazebo::Client *client_;
	gazebo::CameraIface *camera_;
	int imagebytes;
  };
}

#endif
