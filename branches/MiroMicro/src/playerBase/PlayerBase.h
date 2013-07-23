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
#ifndef PlayerBase_h
#define PlayerBase_h

#include "miro/Server.h"
#include "miro/Log.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"

#include "miro/RangeSensorImpl.h"
#include "PlayerLaserImpl.h"
#include "PlayerReactorTask.h"
#include "PlayerMotionImpl.h"
#include "PlayerPanTiltImpl.h"
#include "PlayerCameraControlImpl.h"
#include "PlayerStallImpl.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <libplayerc++/playerc++.h>
#include <libplayerc++/playerclient.h>

using namespace PlayerCc;

class PlayerBase : public Miro::Server, public Miro::Log
{
  typedef Miro::Server Super;

  // = DESCRIPTION
  //    This class starts up the BaseImpl registers it at the
  //    naming service and runs the orb

public:
  // Initialization and Termination methods.
  PlayerBase(int argc, char *argv[],PlayerClient* client_, int playerId) throw (CORBA::Exception);
  // Constructor.

  ~PlayerBase();
  // Destructor.

private:
  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent reactor!
  Miro::PlayerReactorTask reactorTask;

  // Notification Service

  /** Channel factory. */
  CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory_;
 /** Channel identifer. */
  CosNotifyChannelAdmin::ChannelID id_;
  /** Initial qos specified to the factory when creating the EC. */
  CosNotification::QoSProperties initialQos_;
  /** Initial admin props specified to the factory when creating the EC. */
  CosNotification::AdminProperties initialAdmin_;
  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;

  Miro::StructuredPushSupplier structuredPushSupplier_;

  Miro::OdometryImpl odometry;
  Miro::BatteryImpl battery;

  Miro::PlayerMotionImpl motion;

  Player::StallImpl  stall;
  Miro::RangeSensorImpl sonar;
  Miro::RangeSensorImpl tactile;
  Miro::RangeSensorImpl infrared;
  Miro::LaserImpl laser;
  Player::PlayerPanTiltImpl panTilt;
  Player::CameraControlImpl cameraControl;
  //  Miro::GripperImpl gripper;

  Miro::Odometry_var pOdometry;
  Miro::Motion_var pMotion;

  Miro::Stall_var pStall;
  Miro::RangeSensor_var pSonar;
  Miro::RangeSensor_var pTactile;
  Miro::RangeSensor_var pInfrared;
  Miro::Laser_var pLaser;
  Miro::Battery_var pBattery;
  Miro::PanTilt_var pPanTilt;
  Miro::CameraControl_var pCameraControl;
  //  Miro::Gripper_var pGripper;

  PlayerClient* playerClient;

};
#endif




