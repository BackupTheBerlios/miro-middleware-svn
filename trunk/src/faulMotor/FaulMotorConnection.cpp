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
    DBG(cout << "Constructing FaulMotorConnection." << endl);
    if(Sparrow::Parameters::instance()->sparrow2003){
       char const * const accMessage = "ac50\0"; // build acceleration packet
       connection2003->writeLeftWheel(accMessage, strlen(accMessage));
       connection2003->writeRightWheel(accMessage, strlen(accMessage));

    }
    else {
    leftWheel_= new FaulTty::Connection(_reactor,
	       new FaulTty::EventHandler(_consumer, OdometryMessage::LEFT),
	       params_->leftWheel);
    rightWheel_ = new FaulTty::Connection(_reactor,
		new FaulTty::EventHandler(_consumer, OdometryMessage::RIGHT),
	       params_->rightWheel);

    char const * const accMessage = "ac50\r\n\0"; // build acceleration packet
    leftWheel_->writeMessage(accMessage);
    rightWheel_->writeMessage(accMessage);

    }

    disable();
    jmp2();
  }

  Connection::~Connection()
  {
    DBG(cout << "Destructing FaulMotorConnection." << endl);
    jmp1();
    disable();
  }

  //-------------------//
  //----- methods -----//
  //-------------------//

  //----------- commands ----------- //

  void
  Connection::setSpeed(short _speedL, short _speedR)
  {
    Miro::Guard guard(mutex_);
    if (disabled_)
      enable();

    char speedMessageL[20];
    char speedMessageR[20];
    short acctestL, acctestR;

    int speedL = (short) (-_speedL * params_->speedConvFactor);//* 112;
    int speedR = (short) (_speedR * params_->speedConvFactor);//* 112;
    double accL, accR;

    double dSpeedL, dSpeedR;
    dSpeedL = -speedL - prevSpeedL;
    dSpeedR = speedR - prevSpeedR;
    accR = ( params_-> maxPosAccel) * 9. / 320.;
    accL = ( params_-> maxPosAccel) * 9. / 320.;
    if ((dSpeedL != 0) && (dSpeedR != 0)) {
      if (abs(dSpeedL) > abs(dSpeedR)) {
	accR = (dSpeedR / dSpeedL *params_-> maxPosAccel) * 9. / 320.;
      }
      else {
	accL = ( dSpeedL / dSpeedR * params_-> maxPosAccel) * 9. / 320.;
      }
    }

    acctestL = abs((short)accL);
    acctestR = abs((short)accR);
//    cout << "maxPosAcc: " << params_-> maxPosAccel <<" AccR: " << acctestR ;
//    cout << " AccL: " << acctestL << endl;

    if(Sparrow::Parameters::instance()->sparrow2003){
      sprintf(speedMessageL, "ac%d", acctestL); // build speed message
      sprintf(speedMessageR, "ac%d", acctestR); // build speed message
      connection2003->writeLeftWheel(speedMessageL, strlen(speedMessageL));
      connection2003->writeRightWheel(speedMessageR, strlen(speedMessageR));
      
      //ACE_OS::sleep(ACE_Time_Value(2));
      sprintf(speedMessageL, "v%d", speedL); // build speed message
      sprintf(speedMessageR, "v%d", speedR); // build speed message
      connection2003->writeLeftWheel(speedMessageL, strlen(speedMessageL));
      connection2003->writeRightWheel(speedMessageR, strlen(speedMessageR));             // send it

    }
    else {
      if (false && abs(acctestL-prevAccL)>2){			//zur datenverringerung
    	sprintf(speedMessageL, "ac%d\r\n", acctestL); // build speed message
	leftWheel_->writeMessage(speedMessageL);
	prevAccL=acctestL;
      }
      if (false && abs(acctestR-prevAccR)>2){
    	sprintf(speedMessageR, "ac%d\r\n", acctestR); // build speed message
	rightWheel_->writeMessage(speedMessageR);
    	prevAccR=acctestR;
      }

      sprintf(speedMessageL, "ac%d\r\nv%d\r\n", acctestL, speedL); // build speed message
      sprintf(speedMessageR, "ac%d\r\nv%d\r\n", acctestR, speedR); // build speed message
      leftWheel_->writeMessage(speedMessageL);
      rightWheel_->writeMessage(speedMessageR);             // send it
    }
    prevSpeedL = -speedL;
    prevSpeedR = speedR;  
  }

  void
  Connection::getSpeed()
  {
  	if(Sparrow::Parameters::instance()->sparrow2003){
		char const * const getSpeedMessage = "gn\0";

		Miro::Guard guard(mutex_);
		connection2003->writeLeftWheel(getSpeedMessage, 2);             // send it
		connection2003->writeRightWheel(getSpeedMessage, 2);
       }
       else {

	char const * const getSpeedMessage = "gn\r\n\0";

	Miro::Guard guard(mutex_);
	leftWheel_->writeMessage(getSpeedMessage);             // send it
	rightWheel_->writeMessage(getSpeedMessage);
    }
    ACE_OS::sleep(ACE_Time_Value(0, 20000));

  }

  void
  Connection::getTicks()
  {
    char const * const getTicksMessage = "pos\r\n\0";

    Miro::Guard guard(mutex_);

    gotTicks_ = 0;

    ACE_Time_Value now = ACE_OS::gettimeofday();
    //ACE_Time_Value nextWrite = std::min(leftWheel_->lastWrite, rightWheel_->lastWrite)
    //  + ACE_Time_Value(0, 18000);
    ACE_Time_Value nextWrite =  ACE_Time_Value(0, 18000);

    if (nextWrite > now)
      ACE_OS::sleep(nextWrite - now);
    if(Sparrow::Parameters::instance()->sparrow2003){
      char const * const getTicksMessage = "pos\0";
      connection2003->writeLeftWheel(getTicksMessage, 3);            // send it
      connection2003->writeRightWheel(getTicksMessage, 3);
    }
    else {
      char const * const getTicksMessage = "pos\r\n\0";
      leftWheel_->writeMessage(getTicksMessage);            // send it
      rightWheel_->writeMessage(getTicksMessage);
    }
  }

  void
  Connection::setBefehl(char const * const befehl)
  {
    char buffer[256];

    Miro::Guard guard(mutex_);
    strncpy(buffer, befehl, 253);
    if(Sparrow::Parameters::instance()->sparrow2003){
	  strcat(buffer,"\0");
   	 connection2003->writeLeftWheel(buffer, strlen(buffer));
	}
	else {
    strcat(buffer,"\r\n\0");
    leftWheel_->writeMessage(buffer);
    }

    // FIXME: we need to specify the wheel to send the command to
  }

  void
  Connection::setPosition(int _left, int _right)
  {
    char bufferL[20];
    char bufferR[20];
    char const * commandL = bufferL;
    char const * commandR = bufferR;
    if(Sparrow::Parameters::instance()->sparrow2003){
	if (_left == 0) {
		commandL = "hor";
		}
		else {
		sprintf(bufferL, "ho%d", _left);
		}

		if (_right == 0) {
		commandR = "ho";
		}
		else {
		sprintf(bufferR, "ho%d", _right);
		}

		connection2003->writeLeftWheel(commandL, strlen(commandL));
		connection2003->writeRightWheel(commandR, strlen(commandR));

	}
	else {
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

		leftWheel_->writeMessage(commandL);
		rightWheel_->writeMessage(commandR);
    }
  }

  void
  Connection::enable()
  {
     if(Sparrow::Parameters::instance()->sparrow2003){
	char const * const getSpeedMessage = "en\0";

	disabled_ = false;
	connection2003->writeLeftWheel(getSpeedMessage, 2);             // send it
	connection2003->writeRightWheel(getSpeedMessage, 2);

        char const * const posOn = "jmp2\0";						//Odo Anschalten
	connection2003->writeLeftWheel(posOn, strlen(posOn));             // send it
	connection2003->writeRightWheel(posOn, strlen(posOn));

    }
    else{
	char const * const getSpeedMessage = "en\r\n\0";

	disabled_ = false;
	leftWheel_->writeMessage(getSpeedMessage);             // send it
	rightWheel_->writeMessage(getSpeedMessage);
    }
  }

  void
  Connection::disable()
  {
  if(Sparrow::Parameters::instance()->sparrow2003){
    char const * const getSpeedMessage = "di\0";
    disabled_ = true;
    connection2003->writeLeftWheel(getSpeedMessage, 2);             // send it
    connection2003->writeRightWheel(getSpeedMessage, 2);

    char const * const posOff = "jmp1\0";                                           // odo ausschalten
    connection2003->writeLeftWheel(posOff, strlen(posOff));             // send it
    connection2003->writeRightWheel(posOff, strlen(posOff));


    }
    else {
    char const * const getSpeedMessage = "di\r\n\0";
    disabled_ = true;
    leftWheel_->writeMessage(getSpeedMessage);             // send it
    rightWheel_->writeMessage(getSpeedMessage);
    }
  }

  void
  Connection::jmp1()					// keine Ododaten
  {
    char const * const getSpeedMessage = "jmp1\r\n\0";

    leftWheel_->writeMessage(getSpeedMessage);             // send it
    rightWheel_->writeMessage(getSpeedMessage);
  }

  void
  Connection::jmp2()					// ododaten so schnell wie möglich
  {
    char const * const getSpeedMessage = "jmp2\r\n\0";

    leftWheel_->writeMessage(getSpeedMessage);             // send it
    rightWheel_->writeMessage(getSpeedMessage);
  }

  void
  Connection::jmp3()					// notaus keine Ododaten und v0
  {
    char const * const getSpeedMessage = "jmp3\r\n\0";

    leftWheel_->writeMessage(getSpeedMessage);             // send it
    rightWheel_->writeMessage(getSpeedMessage);
  }
};
