// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef SparrowConnection_h
#define SparrowConnection_h

#include "SparrowDevice.h"
#include "Parameters.h"

#include "SparrowBaseConnection.h"         // CanConnection, CanMessageClass

//------------------------ SparrowClass ---------------------------//

namespace Sparrow
{
  // forward declaration
  class Consumer;
  class EventHandler;

  /**
   * Class for using the Sparrow robot
   */
  class Connection : public BaseConnection
  {
    typedef BaseConnection Super;

  public:
    Connection(ACE_Reactor * _reactor,
	       Miro::DevEventHandler * _eventHandler,
	       Consumer * _consumer);
    virtual ~Connection();

    void init();
    void fini();

#ifdef DEPRECATED_SPARROW_FEATURE
    void writeTables();
    void readTables();
#endif

    //------------------------------------------------------------------------
    // motor methods
    //------------------------------------------------------------------------

    void setPower(short left, short right);         // set both wheel's power
    void setSpeed(short left, short right);         // set both wheel's speed
    void setSpeedRot(short speed, short rot);       // set speed and rotation

    void setAccelValues(short index, short table1, short table2);
    void getAccelValues();

    void initDrive(short ticksL, short ticksR, short diameter);
    void initMax(short maxAcc, short maxPower, short maxSpeed, double maxTurn); // set the max values
    void motorInit();
    void motorAllOff();
    bool motorAlive();

    //------------------------------------------------------------------------
    // odometry methods
    //------------------------------------------------------------------------

    void getPosition(unsigned short msec, unsigned short times);
    void getDistanceLR(int& left, int& right);
    void setPosition(short x, short y, double dir);
    void setRelativePosition(short rx, short ry, double rdir);
    bool odoAlive();

    //------------------------------------------------------------------------
    // port methods
    //------------------------------------------------------------------------

    void readDigital();
    void writeDigital(short index, unsigned short _and, unsigned short _or);
    unsigned short readAnalog(short index);
    void setDigital(unsigned short port2);
    void portsAllOff();
    bool portsAlive();

    //------------------------------------------------------------------------
    // stall methods
    //------------------------------------------------------------------------

    void initStall(short threashold, short a2, short a1, short a0);
    void stallTimerStart();
    bool isStalled();
    void unstall();
    bool stallAlive();

    //------------------------------------------------------------------------
    // kicker methods
    //------------------------------------------------------------------------

    void kick(unsigned short msec);
    void kickIfBall(unsigned short tryMsec, unsigned short msec, short threashold);
    void kickerAllOff();
    bool kickerAlive();

    //------------------------------------------------------------------------
    // servo methods
    //------------------------------------------------------------------------

    void servoGo(short index, short value);
    void servoAllOff();
    bool servoAlive();

    void setServo(short servo, double rad);  // turn servo to abs. angle (rad)

    //------------------------------------------------------------------------
    // infrared methods
    //------------------------------------------------------------------------

    void infraredGet(unsigned short msec, unsigned short times);
    bool infraredAlive();

    //------------------------------------------------------------------------
    // debug methods
    //------------------------------------------------------------------------

    void status();
    bool debugAlive();

    //------------------------------------------------------------------------
    // test methods
    //------------------------------------------------------------------------

    void testAdd();
    void testResult();


  protected:
    short rad2servo0Ticks(double rad) const;
    short rad2servo1Ticks(double rad) const;

    Consumer * consumer;

    EventHandler * eventHandler;

    int buttonsPollTimerId;

    // motion parameters
    //
    Miro::Mutex motionMutex;
    StateType motionState;
    short translateSpeed;
    short rotateSpeed;
    short leftSpeed;
    short rightSpeed;
    short leftPower;
    short rightPower;

  public:
    int boardReply; // for watchdog / init
  };

  inline
  short
  Connection::rad2servo0Ticks(double rad) const
  {
    short pulse = params_->servo0MidPulse;
    if (rad > 0.)
      pulse += static_cast<short>(rint(rad * params_->deg2ServoTicksL * 180. / M_PI));
    else
      pulse += static_cast<short>(rint(rad * params_->deg2ServoTicksR * 180. / M_PI));
    return pulse;
  }

  inline
  short
  Connection::rad2servo1Ticks(double rad) const
  {
    return params_->servo1MidPulse +
      static_cast<short>(rint(rad * params_->deg2ServoTicksL * 180. / M_PI));
  }

};

#endif

