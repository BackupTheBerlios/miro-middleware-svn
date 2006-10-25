
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

#include <playerc++.h>
#include <playerclient.h>

using namespace PlayerCc;

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

  inline PlayerConnection::PlayerConnection(PtzProxy * _playerPTZ) {
    playerPTZ_=_playerPTZ;
    //set default values
    panTarget_=playerPTZ_->GetPan();
    tiltTarget_=playerPTZ_->GetTilt();
    zoomTarget_=playerPTZ_->GetZoom();
  }
 
  inline bool PlayerConnection::setPan(double pan) {
    try
    {
      playerPTZ_->SetCam(pan,tiltTarget_,zoomTarget_);
      panTarget_=pan;
      return true;
    } catch(PlayerCc::PlayerError & e) {
      return false;
    }
  }

  inline bool PlayerConnection::setTilt(double tilt) {
    try
    {
      playerPTZ_->SetCam(panTarget_,tilt,zoomTarget_);
      tiltTarget_=tilt;
      return true;
    } catch(PlayerCc::PlayerError & e) {
      return false;
    }
  }

  inline bool PlayerConnection::setPanTilt(double pan, double tilt) {
    try
    {
      playerPTZ_->SetCam(pan,tilt,zoomTarget_);
      panTarget_=pan;
      tiltTarget_=tilt;
      return true;
    } catch(PlayerCc::PlayerError & e) {
      return false;
    }
  }

  inline bool PlayerConnection::setZoom(double zoom) {
    try
    {
      playerPTZ_->SetCam(panTarget_,tiltTarget_,zoom);
      zoomTarget_=zoom;
      return true;
    } catch(PlayerCc::PlayerError & e) {
      return false;
    }
  }

  inline double PlayerConnection::getPan() const {
    return playerPTZ_->GetPan();
  }

  inline double PlayerConnection::getTilt() const {
    return playerPTZ_->GetTilt();
  }

  inline double PlayerConnection::getZoom() const {
    return playerPTZ_->GetZoom();
  }
}// namespace Player

#endif //PlayerConnection_h
