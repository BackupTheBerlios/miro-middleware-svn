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

#ifndef PlayerConnection_h
#define PlayerConnection_h

#include <playerclient.h>

namespace Player {

  class PlayerConnection {
    
  public:

    PlayerConnection(PtzProxy * _playerPTZ);
    
    bool setPan(double pan);
    bool setTilt(double tilt);
    bool setPanTilt(double pan, double tilt);
    bool setZoom(double zoom);

    double getPan() const;
    double getTilt() const;
    double getZoom() const;


  protected:
    PtzProxy * playerPTZ_;

    double panTarget_;
    double tiltTarget_;
    double zoomTarget_;

  }; //class PlayerConnection

  inline bool PlayerConnection::setPan(double pan) {
    if (playerPTZ_->SetCam(pan,tiltTarget_,zoomTarget_)==0) {
      panTarget_=pan;
      return true;
    } else {
      return false;
    }
  }

  inline bool PlayerConnection::setTilt(double tilt) {
    if (playerPTZ_->SetCam(panTarget_,tilt,zoomTarget_)==0) {
      tiltTarget_=tilt;
      return true;
    } else {
      return false;
    }
  }

  inline PlayerConnection::PlayerConnection(PtzProxy * _playerPTZ) {
    playerPTZ_=_playerPTZ;
    //set default values
    panTarget_=playerPTZ_->pan;
    tiltTarget_=playerPTZ_->tilt;
    zoomTarget_=playerPTZ_->zoom;
  }

  inline bool PlayerConnection::setPanTilt(double pan, double tilt) {
    if (playerPTZ_->SetCam(pan,tilt,zoomTarget_)==0) {
      panTarget_=pan;
      tiltTarget_=tilt;
      return true;
    } else {
      return false;
    }
  }

  inline bool PlayerConnection::setZoom(double zoom) {
    if (playerPTZ_->SetCam(panTarget_,tiltTarget_,zoomTarget_)==0) {
      zoomTarget_=zoom;
      return true;
    } else {
      return false;
    }
  }

  inline double PlayerConnection::getPan() const {
    return playerPTZ_->pan;
  }

  inline double PlayerConnection::getTilt() const {
    return playerPTZ_->tilt;
  }

  inline double PlayerConnection::getZoom() const {
    return playerPTZ_->zoom;
  }

}// namespace Player

#endif //PlayerConnection_h
