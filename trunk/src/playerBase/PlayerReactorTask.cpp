#include "PlayerReactorTask.h"
#include "Parameters.h"
#include "PlayerLaserImpl.h"
#include "PlayerMotionImpl.h"

#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"
#include "idl/TimeC.h"
#include "miro/TimeHelper.h"
#include "miro/Server.h"

#include <playerclient.h>

namespace Miro {

  using std::string;
  using std::cout;
  using std::cerr;
  using std::endl;

  bool PlayerReactorTask::done=false;

  PlayerReactorTask::PlayerReactorTask(PlayerClient * client, RangeSensorImpl * _pSonar, LaserImpl * _pLaser, RangeSensorImpl * _pInfrared, RangeSensorImpl * _pTactile, OdometryImpl * _pOdometry, BatteryImpl * _pBattery, PlayerMotionImpl * _pMotion) throw (CORBA::Exception) :
    Super(),
    objectID(0),
    pSonar(_pSonar),
    pLaser(_pLaser),
    pInfrared(_pInfrared),
    pTactile(_pTactile),
    pOdometry(_pOdometry),
    pMotion(_pMotion),
    pBattery(_pBattery),
    params_(::Player::Parameters::instance()),
    playerClient(client),
    playerSonar(NULL),
    playerPosition(NULL),
    playerLaser(NULL),
    playerInfrared(NULL),
    playerPower(NULL),
    playerBumper(NULL)
  {

    playerLaser=new LaserProxy(playerClient,0,'r');
    playerSonar=new SonarProxy(playerClient,0,'r');
    playerPosition=new PositionProxy(playerClient,0,'a');
    playerPower=new PowerProxy(playerClient,0,'r');


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

    if (!playerPosition || playerPosition->GetAccess() != 'a') {
      throw (Miro::Exception("Could not get reference to Player Position"));
    }

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
  }

  int PlayerReactorTask::svc() throw (CORBA::Exception) {
    int sonarReadings=8;
    string robotName="Robot";

   
    pMotion->setPlayerPositionProxy(playerPosition);

    while (!done) {
    
      if (playerClient->Read()) {
	throw Miro::EDevIO("Error in connection to Player Simulator");
      }
      
      if (playerClient->fresh) {
	playerClient->fresh=false;

	TimeIDL timestamp;
	timestamp.sec=playerClient->timestamp.tv_sec;
	timestamp.usec=playerClient->timestamp.tv_usec;

	if ((pLaser!=NULL) && (playerLaser!=NULL)) {
	  int laserReadings=0;
	  for (unsigned int i=0; i<params_->laserDescription.group.length(); i++)
	    laserReadings+=params_->laserDescription.group[i].sensor.length();

	  RangeBunchEventIDL * pLaserEvent = new RangeBunchEventIDL();
	  pLaserEvent->sensor.length(laserReadings);
	
	  // iterate through new data

	  for (int i = laserReadings - 1; i >= 0; --i) {
	    int group = 0;
	    unsigned int index = i;
	    
	    if (index >= params_->laserDescription.group[group].sensor.length()) {
	      ++group;
	      index -= params_->laserDescription.group[group-1].sensor.length();
	    }
	    
	    pLaserEvent->sensor[i].group = group;
	    pLaserEvent->sensor[i].index = index;
	    pLaserEvent->sensor[i].range = 
	      long(playerLaser->scan[i][0]*1000);

	    //check the range
	    if (pLaserEvent->sensor[i].range>params_->laserDescription.group[group].description.maxRange)
	      // should return HORIZON_RANGE, but it is ugly ;-)
	      //	      pLaserEvent->sensor[i].range=Miro::RangeSensor::HORIZON_RANGE;
	    pLaserEvent->sensor[i].range=params_->laserDescription.group[group].description.maxRange;
	    else if ((pLaserEvent->sensor[i].range < params_->laserDescription.group[group].description.minRange) &&
		     (pLaserEvent->sensor[i].range>=0))
	    pLaserEvent->sensor[i].range=params_->laserDescription.group[group].description.minRange;
	  }

	  pLaserEvent->time=timestamp;
	  pLaser->integrateData(pLaserEvent);
	  
	} // Laser end
	if ((pOdometry!=NULL) && (playerPosition!=NULL)) {

	  status.position.point.x=long(playerPosition->Xpos()*1000);
	  status.position.point.y=long(playerPosition->Ypos()*1000);
	  status.position.heading=playerPosition->Theta();

	  status.velocity.translation=long(playerPosition->Speed()*1000);
	  status.velocity.rotation=playerPosition->TurnRate();

	  status.time=timestamp;

	  pOdometry->integrateData(status);

	} // odometry end
	if ((pBattery!=NULL) && (playerPower != NULL)) {
	  pBattery->integrateData(playerPower->Charge());
	} // battery end
      }
    }

    while(!done && 0) {
      //TODO: set the delays acording to the sensor frequency got from the xml file
      TimeIDL timestamp;

      //TODO: get timestamp from simulator and not real time
      timeA2C(ACE_OS::gettimeofday(),timestamp);

      try {

	//TODO: check whether the delay between sensor readings is enough
	if (pSonar!=NULL) {
	  RangeBunchEventIDL * pSonarEvent = new RangeBunchEventIDL();
	  pSonarEvent->sensor.length(sonarReadings);

	  // iterate through new data
	  
	  for (int i = sonarReadings - 1; i >= 0; --i) {
	    int group = 0;
	    int index = i;
	    
	    // TODO: peoplebot sonars.   Must be generalized!!!
	    if (index >= 8) {
	      if (index < 16)
		++group;
	      index -= 8;
	    }
	    
	    //position relative to the robot's center
	    //	    Position3DIDL pos;
	    SensorPositionIDL scanDescription=params_->sonarDescription.group[group].sensor[index];
	    
	    //	    pos.point.x=scanDescription.distance*cos(scanDescription.alpha);
	    //	    pos.point.y=scanDescription.distance*sin(scanDescription.alpha);
	    //	    pos.point.z=scanDescription.height;
	    //	    pos.heading=scanDescription.alpha+scanDescription.beta; //angle relative to the robot's heading and distance vector
	    
	    pSonarEvent->sensor[i].group = group;
	    pSonarEvent->sensor[i].index = index;
	    //	    pSonarEvent->sensor[i].range = 
	    //	      (CORBA::Long) (pSimulator->getDistance(objectID,pos,params_->sonarDescription.group[group].description.maxRange,timestamp));

	    //check the range
	    if (pSonarEvent->sensor[i].range>params_->sonarDescription.group[group].description.maxRange)
	      //Should return HORIZON_RANGE, but it is ugly ;-)
	      //	      pSonarEvent->sensor[i].range=Miro::RangeSensor::HORIZON_RANGE;
	    pSonarEvent->sensor[i].range=params_->sonarDescription.group[group].description.maxRange;

	    else if ((pSonarEvent->sensor[i].range < params_->sonarDescription.group[group].description.minRange) &&
		     (pSonarEvent->sensor[i].range>=0))
	      pSonarEvent->sensor[i].range=params_->sonarDescription.group[group].description.minRange;
	  }
	  pSonarEvent->time=timestamp;
	  pSonar->integrateData(pSonarEvent);
	  
	} // Sonar end

      }
      catch (const CORBA::Exception & e) {
	cerr << "CORBA Exception: ";
	//	::operator<<(cerr,e);
	cerr << endl;
	cerr << "Ending..." << endl;
	throw;
      }

    } //while end

    return 1;
  }

};
