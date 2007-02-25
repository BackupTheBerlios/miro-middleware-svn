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
#ifndef SparrowConnection_h
#define SparrowConnection_h

#include "SparrowDevice.h"
#include "sparrow/Parameters.h"

#include "SparrowBaseConnection.h"         // CanConnection, CanMessageClass
#include "can/Can0Message.h"

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
    typedef Can::Can0Message CanMessage;

  public:
    Connection(ACE_Reactor * _reactor,
	       Miro::DevEventHandler * _eventHandler,
	       Consumer * _consumer);
    virtual ~Connection();

    //! Inherited method.
    virtual void init();
    virtual void fini();

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
  Connection::rad2servo0Ticks(double rad) const {
    short pulse = params_->servo0MidPulse;
    if (rad > 0.)
      pulse += static_cast<short>(rint(rad * params_->deg2ServoTicksL * 180. / M_PI));
    else
      pulse += static_cast<short>(rint(rad * params_->deg2ServoTicksR * 180. / M_PI));
    return pulse;
  }

  inline
  short
  Connection::rad2servo1Ticks(double rad) const {
    return params_->servo1MidPulse +
      static_cast<short>(rint(rad * params_->deg2ServoTicksL * 180. / M_PI));
  }
}

#endif // SparrowConnection_h

