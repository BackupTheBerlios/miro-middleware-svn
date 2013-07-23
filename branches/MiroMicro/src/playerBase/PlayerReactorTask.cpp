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
#include "PlayerReactorTask.h"
#include "Parameters.h"
#include "PlayerLaserImpl.h"
#include "PlayerMotionImpl.h"
#include "PlayerConnection.h"
#include "PlayerPanTiltImpl.h"
#include "PlayerCameraControlImpl.h"
#include "PlayerStallImpl.h"

#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"
#include "idl/TimeC.h"
#include "miro/TimeHelper.h"
#include "miro/Server.h"

namespace Miro 
{
  using namespace PlayerCc;

  using std::string;
  using std::cout;
  using std::cerr;
  using std::endl;

  using Player::PlayerPanTiltImpl;

  bool PlayerReactorTask::done=false;

  PlayerReactorTask::PlayerReactorTask(PlayerClient * client,
				       int playerId,
				       RangeSensorImpl * _pSonar, 
				       LaserImpl * _pLaser, 
				       RangeSensorImpl * _pInfrared, 
				       RangeSensorImpl * _pTactile, 
				       OdometryImpl * _pOdometry, 
				       PlayerMotionImpl * _pMotion, 
				       BatteryImpl * _pBattery, 
				       PlayerPanTiltImpl * _pPanTilt,
				       Player::CameraControlImpl * _pCameraControl,
				       Player::StallImpl * _pStall
				       ) throw (CORBA::Exception) :
    Super(),
    pSonar_(_pSonar),
    pLaser_(_pLaser),
    pInfrared_(_pInfrared),
    pTactile_(_pTactile),
    pOdometry_(_pOdometry),
    pMotion_(_pMotion),
    pBattery_(_pBattery),
    pPanTilt_(_pPanTilt),
    pCameraControl_(_pCameraControl),
    pStall_(_pStall),
    params_(::Player::Parameters::instance()),
    playerClient(client),
    playerSonar(NULL),
    playerPosition(NULL),
    playerLaser(NULL),
    playerInfrared(NULL),
    playerPower(NULL),
    playerBumper(NULL),
    playerPTZ(NULL)
  {

    //position has to be the first proxy to be created.
    //if the first proxy could not be created (i.e. player does not 
    //  provide a driver), player warnings are returned and data for 
    //  other sensors is unreliable.
    //as a robot without odometry/motion is not useful and playerBase
    //  does not run in such a case; further corrupt data would be
    //  unimportant (in fact, it would not be even read)


    playerPosition=new Position2dProxy(playerClient,playerId);

    //MAY NOT BE NECESSARY HAVE TO TEST
#if 1 == 2
    if (!playerPosition) {
      throw (Miro::Exception("Could not get reference to Player Position"));
    }

    try
      {
	playerPosition->Subscribe(playerId);
      } catch(PlayerCc::PlayerError & e) {
      throw (Miro::Exception("Could not get reference to Player Position"));
    }
#endif 

    playerSonar=new SonarProxy(playerClient,playerId);
    playerLaser=new LaserProxy(playerClient,playerId);
    playerPower=new PowerProxy(playerClient,playerId);

    MIRO_DBG_OSTR(MIRO, LL_DEBUG, "Sean Got here\n");

    //playerPTZ=new PtzProxy(playerClient,playerId);

    MIRO_DBG_OSTR(MIRO, LL_DEBUG, "Sean Got here 2\n");

#if 1 == 2
    if (!playerLaser || playerLaser->GetAccess() != 'r') {
      cerr << "WARNING: Could not get reference to Player Laser. No Laser available" << endl;
      delete playerLaser;
      playerLaser=NULL;
    }
    if (!playerSonar || playerSonar->GetAccess() != 'r') {
      cerr << "WARNING: Could not get reference to Player Sonar. No Sonar available" << endl;
      delete playerSonar;
      playerSonar=NULL;
    }
    if (!playerPower || playerPower->GetAccess() != 'r') {
      cerr << "WARNING: Could not get reference to Player Power. No Battery available" << endl;
      delete playerPower;
      playerPower=NULL;
    }
    if (!playerPTZ || playerPTZ->GetAccess() != 'a') {
      cerr << "WARNING: Could not get reference to Player PanTilt. No PanTilt available" << endl;
      delete playerPTZ;
      playerPTZ=NULL;
    }
#endif 

    status.position.point.x=0;
    status.position.point.y=0;
    status.position.heading=0;
    status.velocity.translation=0;
    status.velocity.rotation=0;
  }

  PlayerReactorTask::~PlayerReactorTask()
  {

    delete playerLaser;
    delete playerSonar;
    delete playerPosition;
    delete playerPower;
    delete playerPTZ;
  }

  int PlayerReactorTask::svc() throw (CORBA::Exception) {
    
    double tmpTime;
    assert(pMotion_!=NULL);

    //Do an initial read, to allow initialization with real values
    try {
      playerClient->Read();
    } catch (PlayerCc::PlayerError & e) {
      throw Miro::EDevIO("Error in connection to Player Simulator");
    }

    pMotion_->setPlayerPositionProxy(playerPosition);
    

    Player::PlayerConnection * playerConnection=NULL;
    if (playerPTZ) {
      playerConnection=new Player::PlayerConnection(playerPTZ);
    }
    
    if (pPanTilt_ && playerConnection)
      pPanTilt_->setPlayerConnection(playerConnection);

    if (pCameraControl_ && playerConnection)
      pCameraControl_->setPlayerConnection(playerConnection);

    while (!done) {
   

      try {
        playerClient->Read();
      } catch (PlayerCc::PlayerError & e) {
	throw Miro::EDevIO("Error in connection to Player Simulator");
      }

      //DON"T THINK WE NEED THE FRESH IT WILL ONLY RETURN FROM READ WHEN NEW DATA IS AVAILIABLE SEAN
      //if (clientProxy->IsFresh()) {
      //	clientProxy->NotFresh();


      //*******HOW DO WE DEAL WITH TIME SEAN
      TimeIDL timestamp;
      ///timestamp.sec=playerClient->timestamp.tv_sec;
      //timestamp.usec=playerClient->timestamp.tv_usec;
      //

      if ((pLaser_!=NULL) && (playerLaser!=NULL) && (playerLaser->IsFresh())) {
	playerLaser->NotFresh();

	pLaser_->setScanDescription(params_->laserDescription);

	int laserReadings=playerLaser->GetCount();

	RangeGroupEventIDL * pLaserEvent = new RangeGroupEventIDL();
	pLaserEvent->group = 0;
	pLaserEvent->range.length(laserReadings);

	// iterate through new data

// 	cout << "Laser Readings = " << laserReadings << endl;

	for (int i = laserReadings - 1; i >= 0; --i) {

	  int group = 0;
	  unsigned int index = i;
	    
	  if (index >= params_->laserDescription.group[group].sensor.length()) {
	    ++group;
	    index -= params_->laserDescription.group[group-1].sensor.length();
	  }

	  pLaserEvent->range[i] = 
	    long(playerLaser->GetRange(i)*1000); //Player uses m not mm

	  if (pLaserEvent->range[i] > params_->laserDescription.group[group].description.maxRange)
	    // should return HORIZON_RANGE, but it is ugly ;-)
	    //	      pLaserEvent->sensor[i].range=Miro::RangeSensor::HORIZON_RANGE;
	    pLaserEvent->range[i] =params_->laserDescription.group[group].description.maxRange;
	  else if ((pLaserEvent->range[i] < params_->laserDescription.group[group].description.minRange) &&
		   (pLaserEvent->range[i] >=0))
	    pLaserEvent->range[i] =params_->laserDescription.group[group].description.minRange;
	}

	tmpTime = playerLaser->GetDataTime();
	
	timestamp.sec = floor(tmpTime);
	timestamp.usec =  floor((tmpTime - timestamp.sec) * 1000000);
	pLaserEvent->time=timestamp;

	pLaser_->integrateData(pLaserEvent);
	  
      } // Laser end

      if ((pSonar_!=NULL) && (playerSonar!=NULL) && (playerSonar->IsFresh())) {
	playerSonar->NotFresh();

	RangeBunchEventIDL * pSonarEvent = new RangeBunchEventIDL();

	int sonarReadings=playerSonar->GetCount();
	pSonarEvent->sensor.length(sonarReadings);

	// iterate through new data
	  
	for (int i = sonarReadings - 1; i >= 0; --i) {
	  int group = 0;
	  int index = i;
	    
	  SensorPositionIDL scanDescription=params_->sonarDescription.group[group].sensor[index];
	    
	  pSonarEvent->sensor[i].group = group;
	  pSonarEvent->sensor[i].index = index;
	  pSonarEvent->sensor[i].range = long(playerSonar->GetScan(i)*1000);

	  //check the range
	  if (pSonarEvent->sensor[i].range>params_->sonarDescription.group[group].description.maxRange)
	    //Should return HORIZON_RANGE, but it is ugly ;-)
	    //	      pSonarEvent->sensor[i].range=Miro::RangeSensor::HORIZON_RANGE;
	    pSonarEvent->sensor[i].range=params_->sonarDescription.group[group].description.maxRange;

	  else if ((pSonarEvent->sensor[i].range < params_->sonarDescription.group[group].description.minRange) &&
		   (pSonarEvent->sensor[i].range>=0))
	    pSonarEvent->sensor[i].range=params_->sonarDescription.group[group].description.minRange;
	}

	tmpTime = playerSonar->GetDataTime();
	
	timestamp.sec = floor(tmpTime);
	timestamp.usec =  floor((tmpTime - timestamp.sec) * 1000000);
	pSonarEvent->time=timestamp;

	pSonar_->integrateData(pSonarEvent);
	  
      } // sonar end
	
      if ((pOdometry_!=NULL) && (playerPosition!=NULL) && (playerPosition->IsFresh())) {
	
	playerPosition->NotFresh();

/*	cout << 
	  "player x " << playerPosition->GetXPos() << 
	  "player y " << playerPosition->GetYPos() <<
	  "player heading " << playerPosition->GetYaw() << endl;
*/
	status.position.point.x=long(playerPosition->GetXPos()*1000); //m to mm
	status.position.point.y=long(playerPosition->GetYPos()*1000); //m to mm
	status.position.heading=playerPosition->GetYaw();

	//assure that Theta is between -PI and PI
	while (status.position.heading>M_PI)
	  status.position.heading -= 2*M_PI;
	while (status.position.heading<-M_PI)
	  status.position.heading += 2*M_PI;

	
	// this needs to be tested SEAN
	status.velocity.translation=long(1000*sqrt(playerPosition->GetXSpeed()*playerPosition->GetXSpeed()+playerPosition->GetYSpeed()*playerPosition->GetYSpeed())); 
	// m/s to mm/s

	// is this right???? SEAN
	status.velocity.rotation=playerPosition->GetYawSpeed();

	tmpTime = playerLaser->GetDataTime();
	
	timestamp.sec = floor(tmpTime);
	timestamp.usec =  floor((tmpTime - timestamp.sec) * 1000000);
	status.time=timestamp;

	pOdometry_->integrateData(status);

	if (pStall_!=NULL) {
	  pStall_->integrateData(playerPosition->GetStall());
	}

      } // odometry end

      if ((pBattery_!=NULL) && (playerPower != NULL) && (playerPower->IsFresh())) {
	playerPower->NotFresh();
	pBattery_->integrateData(playerPower->GetCharge());
      } // battery end
	//} // if fresh end SEAN
    }

    return 1;
  }
}
