// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

// This implements an object-oriented user interface to the FaulMotor
// robot.

#include "FaulMotorConnection.h"
#include "FaulMotorConsumer.h"
#include "Parameters.h"
#include "sparrow/Parameters.h"

#include "faulTty/FaulTtyEventHandler.h"
#include "faulTty/FaulCanConnection.h"
#include "faulTty/FaulTtyConnection.h"

#include <cmath>

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif

// Roland: Mittelfristig waere mir folgende Loesung lieber
// anstatt bei jedem write zu unterscheiden, ob wir einen neuen oder alten
// roboter haben:
// FaulTty::write -> FaulController::virtual write
// class FaulTty : public FaulController
// class FaulTtyCanProxy : public FaulController
// write jeweils implementieren.

// if (sparrow2003) nur im constructor

namespace 
{
  char const * const DEF_ACC_MSG[2] = { "ac50", NULL}; // default acceleration
  char const * const GET_SPPED_MSG[2] = { "gn", NULL };
  char const GET_TICKS_MSG[1] = { (char const)201 };
  char const * const MOTOR_ENABLE_MSG[2] = { "en", NULL };
  char const * const MOTOR_DISABLE_MSG[2] = { "di", NULL };
  char const * const DISABLE_ODO_MSG[2] = { "jmp1", NULL };
  char const * const ENABLE_ODO_MSG[2] = { "jmp2", NULL };
  char const * const STOP_MSG[2] = { "jmp3", NULL };
}

namespace FaulMotor
{
  using namespace FaulController;
  using Miro::Guard;

  using std::cout;
  using std::cerr;
  using std::endl;

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
    nextSceduledQuery_(ACE_OS::gettimeofday()),
    writeThisRound_(false),
    prevSpeedL(0),
    prevSpeedR(0),
    prevAccL(0),
    prevAccR(0)
  {
    DBG(cout << "Constructing FaulMotorConnection." << endl);
    if(Sparrow::Parameters::instance()->sparrow2003){

       leftWheel_ = new FaulController::FaulCanConnection(connection2003, Sparrow::Connection2003::LEFT_MOTOR);
       rightWheel_ = new FaulController::FaulCanConnection(connection2003, Sparrow::Connection2003::RIGHT_MOTOR);

       /*char const * const accMessage = "ac50\0"; // build acceleration packet
       connection2003->writeLeftWheel(accMessage, strlen(accMessage));
       connection2003->writeRightWheel(accMessage, strlen(accMessage));*/

    }
    else {
       leftWheel_= new FaulController::FaulTtyConnection(_reactor,
	       new FaulController::EventHandler(_consumer, OdometryMessage::LEFT),
	       params_->leftWheel);
       rightWheel_ = new FaulController::FaulTtyConnection(_reactor,
		new FaulController::EventHandler(_consumer, OdometryMessage::RIGHT),
	       params_->rightWheel);

    }

    leftWheel_->writeMessage(DEF_ACC_MSG);
    rightWheel_->writeMessage(DEF_ACC_MSG);


    disable();
    enableBinary();
    if (!params_->odometryPolling) {
      jmp2();
    }
  }

  Connection::~Connection()
  {
    DBG(cout << "Destructing FaulMotorConnection." << endl);
    if (!params_->odometryPolling) {
      jmp1();
    }
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

    // the first ticks package is ignored, so better get on with it
    getTicks();
  }

  void
  Connection::setSpeed(short _speedL, short _speedR)
  {
    Miro::Guard guard(mutex_);
    if (disabled_)
      enable();

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
    if ((dSpeedL != 0) && (dSpeedR != 0)) {
      if (fabs(dSpeedL) > fabs(dSpeedR)) {
	accR = (dSpeedR / dSpeedL * accR) * 9. / 320.;
      }
      else {
	accL = ( dSpeedL / dSpeedR * accL) * 9. / 320.;
      }
    }

    newAccL = abs((int)rint(accL));
    newAccR = abs((int)rint(accR));

    // if we did not write yet and have enough
    // time left: set the speed directly

    ACE_Time_Value now = ACE_OS::gettimeofday();
    if (!writeThisRound_ &&
	now + ACE_Time_Value(0, 20000) < nextSceduledQuery_) {
      writeThisRound_ = true;
      protectedDeferredSetSpeed();
    }
  }

  void
  Connection::getSpeed()
  {
    Miro::Guard guard(mutex_);
    leftWheel_->writeMessage(GET_SPPED_MSG);             // send it
    rightWheel_->writeMessage(GET_SPPED_MSG);
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
  Connection::setBefehl(char const * const befehl)
  {
    char buffer[256];

    Miro::Guard guard(mutex_);
    strncpy(buffer, befehl, 253);

    strcat(buffer,"\0");
    //    leftWheel_->writeMessage(buffer);


    // FIXME: we need to specify the wheel to send the command to
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
  Connection::jmp1()					// keine Ododaten
  {
    leftWheel_->writeMessage(DISABLE_ODO_MSG);             // send it
    rightWheel_->writeMessage(DISABLE_ODO_MSG);
  }

  void
  Connection::jmp2()					// ododaten so schnell wie m,Av(Bglich
  {
    leftWheel_->writeMessage(ENABLE_ODO_MSG);             // send it
    rightWheel_->writeMessage(ENABLE_ODO_MSG);
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
  Connection::deferredSetSpeed(ACE_Time_Value const& _now)
  {
    Miro::Guard guard(mutex_);

    nextSceduledQuery_ = _now + Parameters::instance()->odometryPace;

    writeThisRound_ = false;
    protectedDeferredSetSpeed();
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
