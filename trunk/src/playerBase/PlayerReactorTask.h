#ifndef PlayerReactorTask_h
#define PlayerReactorTask_h

#include "miro/Task.h"
#include "idl/PositionC.h"
#include "idl/VelocityC.h"
#include "idl/MotionStatusC.h"

#include <string>

//Player forward declarations
class PlayerClient;
class SonarProxy;
class PositionProxy;
class LaserProxy;
class PowerProxy;
class BumperProxy;
class IRProxy;

namespace Player {
  class Parameters;
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
		      RangeSensorImpl * _pSonar=NULL, 
		      LaserImpl * _pLaser=NULL, 
		      RangeSensorImpl * _pInfrared=NULL,
		      RangeSensorImpl * _pTactile=NULL,
		      OdometryImpl * _pOdometry=NULL, 
		      BatteryImpl * _pBattery=NULL,
		      PlayerMotionImpl * _pMotion=NULL) throw (CORBA::Exception);

    virtual ~PlayerReactorTask();

    virtual int svc() throw (CORBA::Exception);

    void setSonar(RangeSensorImpl * _pSonar);
    void setLaser(LaserImpl * _pLaser);
    void setInfrared(RangeSensorImpl * _pInfrared);
    void setTactile(RangeSensorImpl * _pTactile);
    void setOdometry(OdometryImpl * _pOdometry);
    void setMotion(PlayerMotionImpl * _pMotion);

    bool sonarBound();
    bool laserBound();
    bool infraredBound();
    bool tactileBound();
    bool odometryBound();
    bool motionBound();
    bool stallBound();
    bool batteryBound();

    static bool done;

  private:
    int objectID; //ID on the simulator

    RangeSensorImpl * pSonar;
    LaserImpl * pLaser;
    RangeSensorImpl * pInfrared;
    RangeSensorImpl * pTactile;
    OdometryImpl * pOdometry;
    PlayerMotionImpl * pMotion;
    BatteryImpl * pBattery;


    PositionIDL position;
    VelocityIDL velocity;
    MotionStatusIDL status;

    ::Player::Parameters * params_;

    std::string playerHost;
    int playerPort;

    PlayerClient* playerClient;
    SonarProxy* playerSonar;
    PositionProxy* playerPosition;
    LaserProxy* playerLaser;
    IRProxy* playerInfrared;
    PowerProxy* playerPower;
    BumperProxy* playerBumper;
    
  };
  
  inline void PlayerReactorTask::setSonar(RangeSensorImpl * _pSonar) { pSonar=_pSonar; }
  inline void PlayerReactorTask::setLaser(LaserImpl * _pLaser) {pLaser=_pLaser; }
  inline void PlayerReactorTask::setInfrared(RangeSensorImpl * _pInfrared) { pInfrared=_pInfrared; }
  inline void PlayerReactorTask::setTactile(RangeSensorImpl * _pTactile) { pTactile=_pTactile; }
  inline void PlayerReactorTask::setOdometry(OdometryImpl * _pOdometry) {pOdometry=_pOdometry;}
  inline void PlayerReactorTask::setMotion(PlayerMotionImpl * _pMotion) {pMotion = _pMotion; }

  inline bool PlayerReactorTask::sonarBound() { return (pSonar !=NULL ) && (playerSonar != NULL); }
  inline bool PlayerReactorTask::laserBound() { return (pLaser !=NULL ) && (playerLaser != NULL); }
  inline bool PlayerReactorTask::infraredBound() { return (pInfrared !=NULL ) && (playerInfrared != NULL); }
  inline bool PlayerReactorTask::tactileBound() { return (pTactile !=NULL ) && (playerBumper != NULL); }
  inline bool PlayerReactorTask::batteryBound() { return (pBattery != NULL) && (playerPower !=NULL); }
  inline bool PlayerReactorTask::stallBound() { return false; } //(pStall != NULL ) && (playerPosition != NULL); }

};

#endif
