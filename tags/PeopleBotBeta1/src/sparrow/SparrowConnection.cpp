// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "SparrowConnection.h"
#include "SparrowEventHandler.h"
#include "SparrowConsumer.h"
#include "SparrowDevice.h"
#include "SparrowParameters.h"

#include "can/CanMessage.h"

#include "miro/Exception.h"

#include <ace/Reactor.h>

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif

namespace
{
  ACE_Time_Value stallReset(2, 0); 
};

namespace Sparrow
{
  using Can::Message;

  ACE_Time_Value maxTimeout(0, 500000);

  //------------------------//
  //----- constructors -----//
  //------------------------//

  Connection::Connection(ACE_Reactor* _reactor, 
			 Miro::DevEventHandler* _devEventHandler,
			 //			 EventHandler _eventHandler,
			 Consumer * _consumer) : 
    Super(_reactor, _devEventHandler, *Parameters::instance()),
    consumer(_consumer),
    reactor(_reactor),
    eventHandler(new EventHandler(*this)),
    params_(Parameters::instance()),
    buttonsPollTimerId(0),
    motionMutex(),
    motionState(LIMP),
    translateSpeed(0),
    rotateSpeed(0),
    leftSpeed(0),
    rightSpeed(0),
    leftPower(0),
    rightPower(0),
    boardReply(-1)
  { 
    DBG(cout << "Constructing SparrowConnection." << endl);

    // init odometry
    setPosition(0, 0, 0.);

    // Complete initialization after entering of the reactor processing loop.
    // So we start immediately after the start of the reactor
    ACE_Time_Value startReports(0, 1); 
    if (reactor->schedule_timer(eventHandler, 
				(void *)INIT_TIMER, // timer id
				startReports,       // delay
				ACE_Time_Value(5)) // interval
	== -1)
      throw Miro::ACE_Exception(errno, "Failed to register timer for status report startup.");

    
    DBG(cout << "polling buttons every " << params_->buttonsPollInterval << "sec" << endl);
    if ( (buttonsPollTimerId = reactor->schedule_timer(eventHandler, 
				(void *)BUTTONS_TIMER, // timer id
				params_->buttonsPollInterval, // delay
				params_->buttonsPollInterval) ) // interval
	== -1)
      throw Miro::ACE_Exception(errno, "Failed to register timer for buttons polling.");
  }

  void 
  Connection::init() 
  {
    // init motors
    initDrive(params_->ticksL, params_->ticksR, params_->diameter);

    initMax(params_->maxAccel, 
	    params_->maxPower, 
	    params_->maxTransVelocity,
	    params_->maxRotVelocity);

    // init stall
    initStall(params_->stallThreshold, 
	      params_->stallA2, 
	      params_->stallA1,
	      params_->stallA0);

    // init motor control tables
    writeTables();
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//

  Connection::~Connection()
  { 
    DBG(cout << "Destructing SparrowConnection." << endl);

    getPosition(params_->odometryInterval, 1);
    infraredGet(params_->infraredInterval, 1);
    setPower(0, 0);

    reactor->cancel_timer(buttonsPollTimerId);
    delete eventHandler;
  }

  //-------------------//
  //----- methods -----//
  //-------------------//

  void
  Connection::writeTables()
  {
    if (params_->writeTables) {
      int minimum = std::min(params_->table1.size(),
			     params_->table2.size());
      cout << "writing tables of size: " << minimum << " " << flush;
      for (int j = 0; j < minimum; ++j) {
	if (!(j % 30))
	  cout << "." << flush;
	setAccelValues(j, params_->table1[j], params_->table2[j]);
      }
      cout << endl;
    }
  }

  void 
  Connection::readTables()
  {
    if(params_->writeTables) {
      cout << "receiving tables:" << endl;
      consumer->accelMutex.acquire();
      cout << "got mutex" << endl;
      getAccelValues();
      cout << "wait on condition" << endl;
      consumer->accelCond.wait();
      consumer->accelMutex.release();
      
      cout << "table 1" << endl;
      short * table1 = consumer->getTable1();
      for (int i = 0; i < Sparrow::ACCEL_TABLE_SIZE; ++i) {
	cout.width(7);
	cout.fill(' ');
	cout << table1[i];
	if ( ((i+1) % 8) == 0)
	  cout << endl;
      }
      
      cout << endl << endl << "table 2" << endl;
      
      short * table2 = consumer->getTable2();
      for (int i = 0; i < Sparrow::ACCEL_TABLE_SIZE; ++i) {
	cout.width(7);
	cout.fill(' ');
	cout << table2[i];
	if ( ( (i+1) % 8) == 0)
	  cout << endl;
      }
    }
  }

  //----------- commands ----------- //

  // set absolute power for both wheels    //
  // positive values are forward, negatives backward //
  void Connection::setPower(short left, short right)
  {
    Miro::Guard guard(motionMutex);
    if (motionState != POWER || leftPower != left || rightPower != right) {
      motionState = POWER;
      leftPower = left;
      rightPower = right;

      Message message;
      message.length(8);
      message.id(CAN_SET_POWER);
      message.shortData(0, left);
      message.shortData(2, right);
      message.longData(4, 0);

      write(message);
    }
  }

  // set absolute speed in mm/sec for both wheels    //
  // positive values are forward, negatives backward //
  void 
  Connection::setSpeed(short left, short right)
  {
    Miro::Guard guard(motionMutex);
    if (motionState != SPEED || leftSpeed != left || rightSpeed != right) {
      motionState = SPEED;
      leftSpeed = left;
      rightSpeed = right;

      Message message;
      message.length(8);
      message.id(CAN_SET_SPEED);
      message.shortData(0, left);
      message.shortData(2, right);
      message.longData(4, 0);
      
      write(message);
    }
  }

  void 
  Connection::setSpeedRot(short speed, short rot)
  {
    Miro::Guard guard(motionMutex);
    if (motionState != SPEED_ROT || 
	translateSpeed != speed || rotateSpeed != rot) {
      motionState = SPEED_ROT;
      translateSpeed = speed;
      rotateSpeed = rot;

      Can::Message message;
      message.length(8);
      message.id(CAN_SET_SPEED_ROT);
      message.shortData(0, speed);
      message.shortData(2, rot);
      message.longData(4, 0);
      
      write(message);
    }
  }

  void
  Connection::setAccelValues(short index, short table1, short table2)
  {
    Message message;
    message.length(6);
    message.id(CAN_SET_ACCELS);
    message.shortData(0, index);
    message.shortData(2, table1);
    message.shortData(4, table2);

    write(message);
  }

  void
  Connection::getAccelValues()
  {
    Message message;
    message.length(0);
    message.id(CAN_GET_ACCELS);

    write(message);
  }

  void
  Connection::initDrive(short ticksL, short ticksR, short diameter)
  {
    Message message;
    message.length(6);
    message.id(CAN_INIT_DRIVE);
    message.shortData(0, ticksL);
    message.shortData(2, ticksR);
    message.shortData(4, diameter);

    write(message);
  }

  void
  Connection::initMax(short maxAcc, short maxPower, short maxSpeed, double maxTurn)
  {
    Message message;
    message.length(8);
    message.id(CAN_INIT_MAX);
    message.shortData(0, maxAcc);
    message.shortData(2, maxPower);
    message.shortData(4, maxSpeed);
    message.shortData(6, rad2ticks(maxTurn));

    write(message);
  }

  void
  Connection::motorInit()
  {
    Message message;
    message.length(0);
    message.id(CAN_MOTOR_INIT);

    write(message);

  }

  void
  Connection::motorAllOff()
  {
    Message message;
    message.length(0);
    message.id(CAN_MOTOR_ALL_OFF);

    write(message);
  }

  bool
  Connection::motorAlive()
  {
    Miro::Guard guard(consumer->motorAliveMutex);

    Message message;
    message.length(0);
    message.id(CAN_MOTOR_ALIVE);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);
    return (consumer->motorAliveCond.wait(&timeout) != -1);
  }

  void 
  Connection::getPosition(unsigned short msec, unsigned short times)
  {
    Message message;
    message.length(4);
    message.id(CAN_GET_POS_CONT);
    message.shortData(0, msec);
    message.shortData(2, times);

    write(message);
  }

  void 
  Connection::getDistanceLR(int& left, int& right)
  {
    Miro::Guard guard(consumer->distanceLRMutex);

    Message message;
    message.length(0);
    message.id(0x82020102);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);

    if (consumer->distanceLRCond.wait(&timeout) != -1)
      throw Miro::ETimeOut();

    left = consumer->distanceL;
    right = consumer->distanceR;
  }

  void 
  Connection::setPosition(short x, short y, double theta)
  {
    Message message;
    message.length(6);
    message.id(CAN_SET_POS);
    message.shortData(0, (unsigned short)(x));
    message.shortData(2, (unsigned short)(y));
    message.shortData(4, (unsigned short)rad2ticks(theta));

    write(message);
  }

  void 
  Connection::setRelativePosition(short dx, short dy, double delta)
  {
    Message message;
    message.length(6);
    message.id(CAN_SET_POS_REL);
    message.shortData(0, (unsigned short)(dx));
    message.shortData(2, (unsigned short)(dy));
    message.shortData(4, (unsigned short)rad2ticks(delta));

    write(message);
  }

  bool
  Connection::odoAlive()
  {
    Miro::Guard guard(consumer->odoAliveMutex);

    Message message;
    message.length(0);
    message.id(CAN_ODO_ALIVE);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);
    return (consumer->odoAliveCond.wait(&timeout) != -1);
  }

  void
  Connection::readDigital()
  {
    Message message;
    message.length(0);
    message.id(CAN_READ_PORTS);

    write(message);
  }

  void
  Connection::writeDigital(short index, unsigned short _and, unsigned short _or)
  {
    Message message;
    message.length(6);
    message.id(CAN_WRITE_PORT);
    message.longData(0, index);
    message.longData(2, _and);
    message.longData(4, _or);

    write(message);
  }


  unsigned short 
  Connection::readAnalog(short index)
  {
    Miro::Guard guard(consumer->analogMutex);

    Message message;
    message.length(2);
    message.id(CAN_READ_ANALOG);
    message.shortData(0, index);

    write(message);

    consumer->analogCond.wait();
  
    return consumer->analog[index];
  }

  void
  Connection::setDigital(unsigned short port2)
  {
    Message message;
    message.length(2);
    message.id(CAN_SET_PORT);
    message.longData(0, port2);

    write(message);
  }

  void
  Connection::portsAllOff()
  {
    Message message;
    message.length(0);
    message.id(CAN_MOTOR_ALL_OFF);

    write(message);
  }

  bool
  Connection::portsAlive()
  {
    Miro::Guard guard(consumer->portsAliveMutex);

    Message message;
    message.length(0);
    message.id(CAN_PORTS_ALIVE);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);
    return (consumer->portsAliveCond.wait(&timeout) != -1);
  }

  void 
  Connection::initStall(short threashold, short a2, short a1, short a0)
  {
    Message message;
    message.length(8);
    message.id(CAN_STALL_INIT);
    message.shortData(0, threashold);
    message.shortData(2, a2);
    message.shortData(4, a1);
    message.shortData(6, a0);

    write(message);
  }

  void
  Connection::stallTimerStart()
  {
    if ( (eventHandler->stallTimerId = 
	  reactor->schedule_timer(eventHandler, 
				  (void *)STALL_TIMER, // timer id
				  stallReset) )         // delay
	 == -1 )
      throw Miro::ACE_Exception(errno, "Failed to register timer for stall flag clearing.");
  }

  bool
  Connection::isStalled()
  {
    return (eventHandler->stallTimerId != 0);
  }

  void
  Connection::unstall()
  {
    if (eventHandler->stallTimerId) {
      reactor->cancel_timer(eventHandler->stallTimerId);
      eventHandler->stallTimerId = 0;

      // send unstall to sparrowBoard - look in documentation
    }
  };

  bool 
  Connection::stallAlive()
  {
    Miro::Guard guard(consumer->stallAliveMutex);

    Message message;
    message.length(0);
    message.id(CAN_STALL_ALIVE);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);
    return (consumer->stallAliveCond.wait(&timeout) != -1);
  }

  void 
  Connection::kick(unsigned short msec)
  {
    Message message;

    message.length(4);
    message.id(CAN_KICK);
    message.shortData(0, 0x00);
    message.shortData(2, msec);
    message.longData(4, 0x00);

    write(message);
  }

  void 
  Connection::kickIfBall(unsigned short tryMsec, unsigned short msec, short threashold)
  {
    Message message;

    message.length(6);
    message.id(CAN_KICK_IF_BALL);
    message.shortData(0, tryMsec);
    message.shortData(2, msec);
    message.shortData(4, threashold);

    write(message);
  }

  void
  Connection::kickerAllOff()
  {
    Message message;
    message.length(0);
    message.id(CAN_KICK_ALL_OFF);

    write(message);
  }

  bool
  Connection::kickerAlive()
  {
    Miro::Guard guard(consumer->kickerAliveMutex);

    Message message;
    message.length(0);
    message.id(CAN_KICK_ALIVE);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);
    return (consumer->kickerAliveCond.wait(&timeout) != -1);
  }

  void 
  Connection::servoGo(short index, short value)
  {
    Message message;

    message.length(4);
    message.id(CAN_SERVO_GO);
    message.shortData(0, index);
    message.shortData(2, value);

    write(message);
  }

  void
  Connection::servoAllOff()
  {
    Message message;
    message.length(0);
    message.id(CAN_MOTOR_ALL_OFF);

    write(message);
  }

  bool
  Connection::servoAlive()
  {
    Miro::Guard guard(consumer->servoAliveMutex);

    Message message;
    message.length(0);
    message.id(CAN_SERVO_ALIVE);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);
    return (consumer->servoAliveCond.wait(&timeout) != -1);
  }

  void 
  Connection::setServo(short servo, double rad)
  {
    Message message;
    message.length(8);
    message.id(CAN_SERVO_GO);
    message.shortData(0, servo);                         // servo number
    message.shortData(2, (servo)? rad2servo1Ticks(rad) : rad2servo0Ticks(rad));
    message.longData(4, 0);

    write(message);
  }

  void 
  Connection::infraredGet(unsigned short msec, unsigned short times)
  {
    Message message;
    message.length(4);
    message.id(CAN_IR_GET_CONT);
    message.shortData(0, msec);
    message.shortData(2, times);

    write(message);
  }

  bool
  Connection::infraredAlive()
  {
    Miro::Guard guard(consumer->irAliveMutex);

    Message message;
    message.length(0);
    message.id(CAN_IR_ALIVE);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);
    return (consumer->irAliveCond.wait(&timeout) != -1);
  }

  void 
  Connection::status()
  {
    Message message;
    message.length(0);
    message.id(CAN_STATUS);

    write(message);
  }

  bool
  Connection::debugAlive()
  {
    Miro::Guard guard(consumer->dbgAliveMutex);

    Message message;
    message.length(0);
    message.id(CAN_DBG_ALIVE);

    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxTimeout;

    write(message);
    return (consumer->dbgAliveCond.wait(&timeout) != -1);
  }

  void
  Connection::testAdd()
  {
    Message message;
    message.length(0);
    message.id(CAN_TEST_ADD);

    write(message);
  }

  void
  Connection::testResult()
  {
    Message message;
    message.length(0);
    message.id(CAN_TEST_RESULT);

    write(message);
  }
};
