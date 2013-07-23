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
#include "PlayerConnection.h"
#include "PlayerCameraControlImpl.h"


namespace Player
{

  CameraControlImpl::CameraControlImpl(const Miro::CameraControlParameters& _cameraParameters) :
    Super(_cameraParameters)
  {
  }

  CameraControlImpl::~CameraControlImpl() {}

  void CameraControlImpl::setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut)
  {
    if (!zoomParameters_.present) {
      throw Miro::EDevIO("Zoom not supported");
    }
    if (!testZoom(value)) {
      throw Miro::EOutOfBounds();
    }
    if (!playerConnection_->setZoom(value)) {
      throw Miro::EDevIO("Error on Player device. Could not zoom");
    } else {
      setTargetZoom(value);
    }
  }
  
  float CameraControlImpl::getZoom() throw(Miro::EDevIO, Miro::ETimeOut) 
  {
    return playerConnection_->getZoom();
  }

  void
  CameraControlImpl::setPlayerConnection(PlayerConnection * _playerConnection)
  {
    playerConnection_=_playerConnection;

    //set a reasonable initial value. Namely, the value the camera currently has
    setTargetZoom(getZoom());
  }

}
