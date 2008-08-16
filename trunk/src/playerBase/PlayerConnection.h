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
#ifndef PlayerConnection_h
#define PlayerConnection_h

#include <libplayerc++/playerc++.h>
#include <libplayerc++/playerclient.h>

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
