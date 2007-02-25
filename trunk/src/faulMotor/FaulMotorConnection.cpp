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
#include "FaulMotorConnection.h"
#include "FaulMotorConsumer.h"
#include "Parameters.h"

#include "sparrow/Parameters.h"

#include "faulTty/FaulTtyEventHandler.h"
#include "faulTty/FaulCanConnection.h"
#include "faulTty/FaulTtyConnection.h"

#include "miro/Log.h"

#include <cmath>

namespace 
{
  char const * const DEF_ACC_MSG[2] = { "ac50", NULL}; // default acceleration
  char const * const GET_SPPED_MSG[2] = { "gn", NULL };
  char const GET_TICKS_MSG[1] = { (char const)201 };
  char const * const MOTOR_ENABLE_MSG[2] = { "en", NULL };
  char const * const MOTOR_DISABLE_MSG[2] = { "di", NULL };
  char const * const STOP_MSG[2] = { "jmp3", NULL };
}

namespace FaulMotor
{
  using namespace FaulController;

  unsigned int Connection::gotTicks_ = 2;

  //------------------------//
  //----- constructors -----//
  //------------------------//

  Connection::Connection(ACE_Reactor* _reactor,
			 Consumer * _consumer,
			 Sparrow::Connection2003 * _connection2003) :
    params_(Parameters::instance()),
    consumer(_consumer),
    connection2003(_connection2003),
    prevSpeedL(0),
    prevSpeedR(0),
    prevAccL(0),
    prevAccR(0)
  {
    MIRO_LOG_CTOR("FaulMotor::Connection");
    if(Sparrow::Parameters::instance()->sparrow2003) {

       leftWheel_ = 
	 new FaulController::FaulCanConnection(connection2003,
					       Sparrow::Connection2003::LEFT_MOTOR);
       rightWheel_ = 
	 new FaulController::FaulCanConnection(connection2003,
					       Sparrow::Connection2003::RIGHT_MOTOR);
    }
    else {
      FaulController::EventHandler * leftHandler =
	new FaulController::EventHandler(_consumer, OdometryMessage::LEFT);
      leftWheel_ =
	new FaulController::FaulTtyConnection(_reactor,
					      leftHandler,
					      params_->leftWheel);
      FaulController::EventHandler * rightHandler =
	new FaulController::EventHandler(_consumer, OdometryMessage::RIGHT);
      rightWheel_ = 
	new FaulController::FaulTtyConnection(_reactor,
					      rightHandler,
					      params_->rightWheel);
    }

    leftWheel_->writeMessage(DEF_ACC_MSG);
    rightWheel_->writeMessage(DEF_ACC_MSG);

    // limp motors
    disable();
    enableBinary();
  }

  Connection::~Connection()
  {
    MIRO_LOG_DTOR("FaulMotor::Connection");
    // limp motors
    disable();
  }

  //-------------------//
  //----- methods -----//
  //-------------------//

  //----------- commands ----------- //

  void
  Connection::enableBinary()
  {
    // data package to enable binary commands
    char const init[4] = {200, 200, 202, 255};

    leftWheel_->writeBinary(init, 4);
    rightWheel_->writeBinary(init, 4);
  }

  void
  Connection::setSpeed(short _speedL, short _speedR)
  {
    Miro::Guard guard(mutex_);
    if (disabled_){
      enable();
      prevSpeedL = 0;
      prevSpeedR = 0;
    }

    // calculate the new velocities
    newSpeedL = (int)rint(-_speedL * params_->speedConvFactor);//* 112;
    newSpeedR = (int)rint(_speedR * params_->speedConvFactor);//* 112;

    // by default use maximum accelaration
    // acceleration conversion is hardcoded - sorry
    double accR =  params_->maxPosAccel * 9. / 320.;
    double accL =  accR;

    // calculate the speed difference
    double dSpeedL = -newSpeedL + prevSpeedL;
    double dSpeedR = newSpeedR - prevSpeedR;

    // if the robot has to change its speed
    // adjust the acceleations accordingly
    if ((dSpeedL != 0) && (dSpeedR != 0) &&
	fabs(newSpeedL) > 250. && fabs(newSpeedR) > 250. ) {
      if (fabs(dSpeedL) > fabs(dSpeedR)) {
	accR *= dSpeedR / dSpeedL;
      }
      else {
	accL *= dSpeedL / dSpeedR;
      }
    }

    newAccL = abs((int)rint(accL));
    newAccR = abs((int)rint(accR));
  }

  void
  Connection::getTicks()
  {
    Miro::Guard guard(mutex_);

    gotTicks_ = 0;
    
    leftWheel_->writeBinary(GET_TICKS_MSG, 1); // send it
    rightWheel_->writeBinary(GET_TICKS_MSG, 1);
  }

  void
  Connection::setPosition(int _left, int _right)
  {
    char bufferL[20];
    char bufferR[20];
    char const * messageL[2] = { bufferL, NULL };
    char const * messageR[2] = { bufferR, NULL };

    if (_left == 0) {
      messageL[0] = "ho";
    }
    else {
      sprintf(bufferL, "ho%d", _left);
    }

    if (_right == 0) {
      messageR[0] = "ho";
    }
    else {
      sprintf(bufferR, "ho%d", _right);
    }
    
    leftWheel_->writeMessage(messageL);
    rightWheel_->writeMessage(messageR);
  }

  void
  Connection::enable()
  {
    disabled_ = false;
    leftWheel_->writeMessage(MOTOR_ENABLE_MSG);             // send it
    rightWheel_->writeMessage(MOTOR_ENABLE_MSG);
  }

  void
  Connection::disable()
  {
    disabled_ = true;
    leftWheel_->writeMessage(MOTOR_DISABLE_MSG);             // send it
    rightWheel_->writeMessage(MOTOR_DISABLE_MSG);

  }

  void
  Connection::jmp3()					// notaus keine Ododaten und v0
  {
    leftWheel_->writeMessage(STOP_MSG);             // send it
    rightWheel_->writeMessage(STOP_MSG);
  }


  /** 
   * Warning, this method is designed as a callback from
   * TimerEventHandler. Everthing else will fail!
   */
  void
  Connection::deferredSetSpeed()
  {
    Miro::Guard guard(mutex_);

    if (Sparrow::Parameters::instance()->sparrow2003){
      gotTicks_ = 0;
      leftWheel_->sendAccVelTicks(newAccL, newAccR, newSpeedL, newSpeedR);
      prevSpeedL = newSpeedL;
      prevSpeedR = newSpeedR;
      prevAccL = newAccL;
      prevAccR = newAccR;
    } 
    else {
      protectedDeferredSetSpeed();
    }
  }

  void
  Connection::protectedDeferredSetSpeed()
  {
    char speedMessageL[20];
    char speedMessageR[20];
    char accMessageL[20];
    char accMessageR[20];
    char const * messageL[3] = { NULL, NULL, NULL };
    char const * messageR[3] = { NULL, NULL, NULL };

    int indexL = 0;
    int indexR = 0;
    if (newAccL != prevAccL) {                //zur datenverringerung
      sprintf(accMessageL, "ac%d", newAccL); // build acc message
      messageL[indexL] = accMessageL;
      ++indexL;
      prevAccL = newAccL;
    }

    if (newSpeedL != prevSpeedL) {
      sprintf(speedMessageL, "v%d", newSpeedL); // build speed message
      messageL[indexL] = speedMessageL;
      ++indexL;
      prevSpeedL = newSpeedL;

    }

    if (newAccR != prevAccR) {                //zur datenverringerung
      sprintf(accMessageR, "ac%d", newAccR); // build acc message
      messageR[indexR] = accMessageR;
      ++indexR;
      prevAccR = newAccR;
    }

    if (newSpeedR != prevSpeedR) {
      sprintf(speedMessageR, "v%d", newSpeedR); // build speed message
      messageR[indexR] = speedMessageR;
      ++indexR;
      prevSpeedR = newSpeedR;

    }

    if (indexL > 0)
      leftWheel_->writeMessage(messageL);
    if (indexR > 0)
      rightWheel_->writeMessage(messageR);   // send it
  }
}
