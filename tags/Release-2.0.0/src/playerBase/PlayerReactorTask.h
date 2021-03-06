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
#ifndef PlayerReactorTask_h
#define PlayerReactorTask_h

#include "miro/Log.h"
#include "miro/Task.h"
#include "idl/PositionC.h"
#include "idl/VelocityC.h"
#include "idl/MotionStatusC.h"

#include <string>

#include <libplayerc++/playerc++.h>
#include <libplayerc++/playerclient.h>

using namespace PlayerCc;

//Player forward declarations
//class PlayerClient;
//class SonarProxy;
//class PositionProxy;
//class LaserProxy;
//class PowerProxy;
//class BumperProxy;
//class IRProxy;
//class PtzProxy;

namespace Player {
  class Parameters;
  class PlayerPanTiltImpl;
  class CameraControlImpl;
  class StallImpl;
}

namespace Miro {

  class RangeSensorImpl;
  class LaserImpl;
  class OdometryImpl;
  class PlayerMotionImpl;
  class BatteryImpl;

  class PlayerReactorTask : public Task {
    typedef Task Super;
  public: 

    PlayerReactorTask(PlayerClient * client, 
		      int playerId=0,
		      RangeSensorImpl * _pSonar=NULL, 
		      LaserImpl * _pLaser=NULL, 
		      RangeSensorImpl * _pInfrared=NULL,
		      RangeSensorImpl * _pTactile=NULL,
		      OdometryImpl * _pOdometry=NULL, 
		      PlayerMotionImpl * _pMotion=NULL,
		      BatteryImpl * _pBattery=NULL,
		      Player::PlayerPanTiltImpl * _pPanTilt=NULL,
		      Player::CameraControlImpl * _pCameraControl=NULL,
		      Player::StallImpl * _pStall=NULL
		      ) throw (CORBA::Exception);

    virtual ~PlayerReactorTask();

    virtual int svc() throw (CORBA::Exception);

    bool sonarBound();
    bool laserBound();
    bool infraredBound();
    bool tactileBound();
    bool odometryBound();
    bool motionBound();
    bool stallBound();
    bool batteryBound();
    bool panTiltBound();
    bool cameraControlBound();

    static bool done;

  private:

    RangeSensorImpl * pSonar_;
    LaserImpl * pLaser_;
    RangeSensorImpl * pInfrared_;
    RangeSensorImpl * pTactile_;
    OdometryImpl * pOdometry_
;
    PlayerMotionImpl * pMotion_;
    BatteryImpl * pBattery_;
    ::Player::PlayerPanTiltImpl * pPanTilt_;
    ::Player::CameraControlImpl * pCameraControl_;
    ::Player::StallImpl * pStall_;

    PositionIDL position;
    VelocityIDL velocity;
    MotionStatusIDL status;

    ::Player::Parameters * params_;

    PlayerClient* playerClient;
    SonarProxy* playerSonar;
    Position2dProxy* playerPosition;
    LaserProxy* playerLaser;
    IrProxy* playerInfrared;
    PowerProxy* playerPower;
    BumperProxy* playerBumper;
    PtzProxy* playerPTZ;
    
  };
  
  inline bool PlayerReactorTask::sonarBound() { return (pSonar_ !=NULL ) && (playerSonar != NULL); }
  inline bool PlayerReactorTask::laserBound() { return (pLaser_ !=NULL ) && (playerLaser != NULL); }
  inline bool PlayerReactorTask::infraredBound() { return (pInfrared_ !=NULL ) && (playerInfrared != NULL); }
  inline bool PlayerReactorTask::tactileBound() { return (pTactile_ !=NULL ) && (playerBumper != NULL); }
  inline bool PlayerReactorTask::odometryBound() { return (pOdometry_ !=NULL ) && (playerPosition != NULL); }
  inline bool PlayerReactorTask::motionBound() { return (pMotion_ !=NULL ) && (playerPosition != NULL); }
  inline bool PlayerReactorTask::batteryBound() { return (pBattery_ != NULL) && (playerPower != NULL); }
  inline bool PlayerReactorTask::stallBound() { return (pStall_ != NULL ) && (playerPosition != NULL); }
  inline bool PlayerReactorTask::panTiltBound() { return (pPanTilt_ != NULL) && (playerPTZ != NULL); }
  inline bool PlayerReactorTask::cameraControlBound() { return (pCameraControl_ != NULL) && (playerPTZ != NULL); }
};

#endif
