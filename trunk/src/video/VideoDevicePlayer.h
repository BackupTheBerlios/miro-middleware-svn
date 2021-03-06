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
#ifndef VideoDevicePlayer_h
#define VideoDevicePlayer_h

#include "VideoDevice.h"

// forward declarations
namespace PlayerCc {
  class PlayerClient;
  class CameraProxy;
}

namespace Video
{
  /// Miro video device which connects to the player robot simulator
  class DevicePlayer : public Device
  {
    typedef Device Super;

  public:
    DevicePlayer(const Miro::ImageFormatIDL& _inputFormatParameters);
    virtual ~DevicePlayer();
	    	    
    FILTER_PARAMETERS_FACTORY(DevicePlayer);

  protected:
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void process();

    PlayerCc::PlayerClient * playerClient_;
    PlayerCc::CameraProxy * playerCamera_;
  };
}

#endif
