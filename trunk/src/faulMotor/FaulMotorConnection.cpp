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

#include "faulTty/FaulTtyEventHandler.h"

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif


namespace FaulMotor
{
  using namespace FaulTty;
  using Miro::Guard;

  //------------------------//
  //----- constructors -----//
  //------------------------//

  Connection::Connection(ACE_Reactor* _reactor, 
			 Consumer * _consumer) : 
    params_(Parameters::instance()),
    leftWheel_(_reactor, 
	       new FaulTty::EventHandler(_consumer, OdometryMessage::LEFT), 
	       params_->leftWheel),
    rightWheel_(_reactor, 
		new FaulTty::EventHandler(_consumer, OdometryMessage::RIGHT), 
	       params_->rightWheel),
    consumer(_consumer)
  { 
    DBG(cout << "Constructing FaulMotorConnection." << endl);

    char const * const accMessage = "ac50\r\n\0"; // build acceleration packet
    leftWheel_.writeMessage(accMessage);
    rightWheel_.writeMessage(accMessage);
  }

  Connection::~Connection()
  { 
    DBG(cout << "Destructing FaulMotorConnection." << endl);
  }

  //-------------------//
  //----- methods -----//
  //-------------------//

  //----------- commands ----------- //

  void
  Connection::setSpeed(short _speed)
  {
    char speedMessageL[20];
    char speedMessageR[20];

    if (_speed < 10) {       // zum bremsen grosse beschl.
      char const * const speedMessage = "ac20\r\n\0";
      leftWheel_.writeMessage(speedMessage);
      rightWheel_.writeMessage(speedMessage);
    }

    int speed = (short) (_speed * params_->speedConvFactor);//* 112;

    sprintf(speedMessageL, "v%d\r\n", -speed); // build speed message
    sprintf(speedMessageR, "v%d\r\n", speed); // build speed message
    leftWheel_.writeMessage(speedMessageL);
    rightWheel_.writeMessage(speedMessageR);             // send it
  }

  void
  Connection::setSpeed(short _speedL, short _speedR)
  {
    char speedMessageL[20];
    char speedMessageR[20];

    int speedL = (short) (-_speedL * params_->speedConvFactor);//* 112;
    int speedR = (short) (_speedR * params_->speedConvFactor);//* 112;

    sprintf(speedMessageL, "v%d\r\n", speedL); // build speed message
    sprintf(speedMessageR, "v%d\r\n", speedR); // build speed message
    leftWheel_.writeMessage(speedMessageL);
    rightWheel_.writeMessage(speedMessageR);             // send it
  }

  void
  Connection::getSpeed()
  {
    char const * const getSpeedMessage = "gn\r\n\0";

    leftWheel_.writeMessage(getSpeedMessage);             // send it
    rightWheel_.writeMessage(getSpeedMessage);
  }

  void
  Connection::getTicks()
  {
    char const * const getTicksMessage = "pos\r\n\0";

    leftWheel_.writeMessage(getTicksMessage);            // send it
    rightWheel_.writeMessage(getTicksMessage);
  }

  void
  Connection::setBefehl(char const * const befehl)
  {
    char buffer[256];
    strncpy(buffer, befehl, 253);
    strcat(buffer,"\r\n\0");
    leftWheel_.writeMessage(buffer);

    // FIXME: we need to specify the wheel to send the command to
  }

  void
  Connection::setPosition(int _left, int _right)
  {
    char bufferL[20];
    char bufferR[20];
    char const * commandL = bufferL;
    char const * commandR = bufferR;

    if (_left == 0) {
      commandL = "ho\n\r";
    }
    else {
      sprintf(bufferL, "ho%d\r\n", _left);
    }

    if (_right == 0) {
      commandR = "ho\n\r";
    }
    else {
      sprintf(bufferR, "ho%d\r\n", _right);
    }

    leftWheel_.writeMessage(commandL);
    rightWheel_.writeMessage(commandR);
  }
};
