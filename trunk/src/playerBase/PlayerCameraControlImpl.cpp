// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
  }

}
