// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "SparrowConsumer.h"
#include "SparrowConnection.h"
#include "SparrowStallImpl.h"
#include "SparrowButtonsImpl.h"

#include "can/Parameters.h"
#include "can/CanMessage.h"

#include "miro/RangeSensorImpl.h"
#include "miro/TimeHelper.h"
#include "miro/Exception.h"
#include "miro/Log.h"

#include "idl/ButtonsC.h"

namespace Sparrow
{
  using Miro::ButtonStatusIDL;

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Connection * _connection,
		     Miro::OdometryImpl * _pOdometry,
		     StallImpl * _pStall,
		     ButtonsImpl * _pButtons,
		     Miro::RangeSensorImpl * _pIR) :
    connection(_connection),
    pOdometry_(_pOdometry),
    pStall_(_pStall),
    pButtons_(_pButtons),
    pIR_(_pIR),
    params_(Parameters::instance()),
    status_(),
    motorAliveMutex(),
    motorAliveCond(motorAliveMutex),
    odoAliveMutex(),
    odoAliveCond(odoAliveMutex),
    portsAliveMutex(),
    portsAliveCond(portsAliveMutex),
    stallAliveMutex(),
    stallAliveCond(stallAliveMutex),
    kickerAliveMutex(),
    kickerAliveCond(kickerAliveMutex),
    servoAliveMutex(),
    servoAliveCond(servoAliveMutex),
    dbgAliveMutex(),
    dbgAliveCond(dbgAliveMutex),
    distanceLRMutex(),
    distanceLRCond(distanceLRMutex),
    digitalMutex(),
    digitalCond(digitalMutex),
    analogMutex(),
    analogCond(analogMutex),
    irAliveMutex(),
    irAliveCond(irAliveMutex),
    accelMutex(),
    accelCond(accelMutex),
    x_(0.),
    y_(0.),
    index_(0)
  {
    MIRO_LOG_CTOR("Sparrow::Consumer");

    status_.position.point.x = 0.;
    status_.position.point.y = 0.;
    status_.position.heading = 0.;
    status_.velocity.translation = 0;
    status_.velocity.rotation = 0.;

    if (pOdometry_) {
      Miro::PositionIDL origin;
      origin.point.x = -params_->initialX;
      origin.point.y = -params_->initialY;
      origin.heading = -params_->initialHeading;

      if (params_->goalie) {
        origin.point.x += 125;
        origin.point.y -= 85;
      }
      pOdometry_->setPosition(origin);
    }
  }

  Consumer::Consumer() :
    params_(Parameters::instance()),
    status_(),
    motorAliveMutex(),
    motorAliveCond(motorAliveMutex),
    odoAliveMutex(),
    odoAliveCond(odoAliveMutex),
    portsAliveMutex(),
    portsAliveCond(portsAliveMutex),
    stallAliveMutex(),
    stallAliveCond(stallAliveMutex),
    kickerAliveMutex(),
    kickerAliveCond(kickerAliveMutex),
    servoAliveMutex(),
    servoAliveCond(servoAliveMutex),
    dbgAliveMutex(),
    dbgAliveCond(dbgAliveMutex),
    distanceLRMutex(),
    distanceLRCond(distanceLRMutex),
    digitalMutex(),
    digitalCond(digitalMutex),
    analogMutex(),
    analogCond(analogMutex),
    irAliveMutex(),
    irAliveCond(irAliveMutex),
    accelMutex(),
    accelCond(accelMutex),
    x_(0.),
    y_(0.),
    index_(0)
  {
    MIRO_LOG_CTOR("Sparrow::Consumer");

    status_.position.point.x = 0.;
    status_.position.point.y = 0.;
    status_.position.heading = 0.;
    status_.velocity.translation = 0;
    status_.velocity.rotation = 0.;


  }


  //----------------------//
  //----- destructor -----//
  //----------------------//
  Consumer::~Consumer()
  {
    MIRO_LOG_DTOR("Sparrow::Consumer");

    motorAliveCond.broadcast();
    odoAliveCond.broadcast();
    portsAliveCond.broadcast();
    stallAliveCond.broadcast();
    kickerAliveCond.broadcast();
    servoAliveCond.broadcast();

    analogCond.broadcast();

    motorAliveMutex.release();
    odoAliveMutex.release();
    portsAliveMutex.release();
    stallAliveMutex.release();
    kickerAliveMutex.release();
    servoAliveMutex.release();

    digitalMutex.release();
    analogMutex.release();
  }


  void Consumer::registerInterfaces(Connection * _connection,
                                    Miro::OdometryImpl * _pOdometry,
                                    StallImpl * _pStall,
                                    ButtonsImpl * _pButtons,
                                    Miro::RangeSensorImpl * _pIR)
  {

     connection = _connection;
     pOdometry_ = _pOdometry;
     pStall_ = _pStall;
     pButtons_ = _pButtons;
     pIR_ = _pIR;

     if (pOdometry_) {
      Miro::PositionIDL origin;
      origin.point.x = -params_->initialX;
      origin.point.y = -params_->initialY;
      origin.heading = -params_->initialHeading;

      if (params_->goalie) {
        origin.point.x += 125;
        origin.point.y -= 85;
      }
      pOdometry_->setPosition(origin);
    }




  }
  // reads incoming packets from the canBus connection and stores the values
  // in the local (class internal) variables.
  void
  Consumer::handleMessage(const Miro::DevMessage * _message)
  {
    const Can::Message& message = *((Can::Message*)_message);
    int tmp;
    int versNr, versSub;

    connection->boardReply = 1;

    Can::Parameters *CanParams = new Can::Parameters();

    CanId msgID = message.id();

    if (CanParams->module == "pcan")
       msgID = (msgID | 0x80000000);

    switch (msgID) {

      // Motor Messages

    case CAN_R_MOTOR_ALIVE: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread:  received message: MOTOR_ALIVE");

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "MOTOR_ALIVE_VERSION: "<< versNr << "." << versSub);
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "MOTOR_TIME_STAMP (sec): " << message.shortData(2));
      Miro::Guard guard(motorAliveMutex);
      motorAliveCond.broadcast();
      break;
    }

    case CAN_R_GET_ACCELS: {
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "Consumer::receiveThread:  received message: GET_ACCELS");

      Miro::Guard guard(accelMutex);

      tmp = index_;
      if (message.shortData(0) != index_) {
	MIRO_LOG_OSTR(LL_ERROR, 
		      "GET_ACCELS lost entries: " << endl
		      << "expected entry: " << index_
		      << "recieved entry: " << message.shortData(0));
      }

      index_ = message.shortData(0);
      table1[index_] = message.shortData(2);
      table2[index_] = message.shortData(4);


      ++index_;
      index_ %= ACCEL_TABLE_SIZE;
      if (index_ < tmp) // we got a whole table
	accelCond.broadcast();
      break;
    }

      // Odometry Messages
    case CAN_R_GET_POS:
      if (pOdometry_) {
	// get data
	xPos_ = message.shortData(0);
	yPos_ = message.shortData(2);
	phi_ = ticks2rad(message.shortData(4));

	// the goalie is special
	if (params_->goalie) {
	  std::swap(xPos_, yPos_);
	  yPos_ *= -1;

	  xPos_ += (short)(125 * cos(phi_) + 85 *  sin(phi_));
	  yPos_ += (short)(125 * sin(phi_) - 85 *  cos(phi_));

	}

	double dx = (short)(xPos_ - xPrev_);
	double dy = (short)(yPos_ - yPrev_);

	x_ += dx;
	y_ += dy;
	double dPhi = phi_ - phiPrev_;

	xPrev_ = xPos_;
	yPrev_ = yPos_;
	phiPrev_ = phi_;

	// fill motion status data struct
	double deltaT = 1000. / (double) (params_->odometryPulse.msec());
	Miro::timeA2C(message.time(), status_.time);
	status_.position.point.x = x_ * params_->transFactor;
	status_.position.point.y = y_ * params_->transFactor;
	status_.position.heading = phi_;
	status_.velocity.translation = (long) rint(sqrt(dx * dx + dy * dy) * deltaT);
	status_.velocity.rotation = dPhi * deltaT;

	pOdometry_->integrateData(status_);
	break;
      }

    case 0x82820201: { // DISTANCE LR
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE, 
		    "Consumer::receiveThread:  received message: DISTANCE LR "
		    << message.shortData(0) << "  "
		    << message.shortData(2));

      Miro::Guard guard(distanceLRMutex);
      distanceL = message.shortData(0);
      distanceR = message.shortData(2);
      distanceLRCond.broadcast();
      break;
    }

    case CAN_R_ODO_ALIVE: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread:  received message: ODO_ALIVE");

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "ODO_ALIVE_VERSION: "<< versNr << "." << versSub);
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "ODO_TIME_STAMP (sec): " << message.shortData(2));
      Miro::Guard guard(odoAliveMutex);
      odoAliveCond.broadcast();
      break;
    }
      // Port Messages

    case CAN_R_READ_PORTS: { // DIGITAL
      MIRO_DBG(SPARROW, LL_PRATTLE,
	       "Consumer::receiveThread:  received message: DIGITAL ");

      Miro::Guard guard(digitalMutex);
      for (int i = 1; i >= 0; --i) {
	if ( ((message.byteData(1) ^ digital[1]) & (0x01 << i)) && pButtons_) {
	  ACE_Time_Value t = ACE_OS::gettimeofday();
	  ButtonStatusIDL * pEvent = new ButtonStatusIDL();
	  pEvent->time.usec = t.usec();
	  pEvent->time.sec = t.sec();
	  pEvent->number = i;
	  pEvent->event = (message.byteData(1) & (0x01 << i))?
	    Miro::Button::ON_RELEASE : Miro::Button::ON_PRESS;
	  pButtons_->pushEvent(pEvent);
	}
      }

      for (int i = 7; i >= 0; --i) {
	digital[i] = message.byteData(i);
      }

      digitalCond.broadcast();
      break;
    }

    case CAN_R_READ_ANALOG: {
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "Consumer::receiveThread:  received message: ANALOG "
		    << message.shortData(0) << "  "
		    << message.shortData(2));

      Miro::Guard guard(analogMutex);
      analog[message.shortData(0)]= message.shortData(2);
      analogCond.broadcast();
      break;
    }

    case CAN_R_PORTS_ALIVE: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread:  received message: PORTS_ALIVE");

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "PORTS_ALIVE_VERSION: "<< versNr << "." << versSub);
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "PORTS_TIME_STAMP (sec): " << message.shortData(2));
      Miro::Guard guard(portsAliveMutex);
      portsAliveCond.broadcast();
      break;
    }

      // Stall Messages

    case CAN_R_STALL:
      cout << "Consumer::receiveThread:  received message: STALL_RETURN" << endl;
      connection->stallTimerStart();
      if (pStall_)
	pStall_->pushEvent();
      break;

    case CAN_R_STALL_ALIVE: {
      MIRO_DBG(SPARROW, LL_PRATTLE,
	       "Consumer::receiveThread:  received message: STALL_ALIVE");

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "STALL_ALIVE_VERSION: "<< versNr << "." << versSub);
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "STALL_TIME_STAMP (sec): " << message.shortData(2));
      Miro::Guard guard(stallAliveMutex);
      stallAliveCond.broadcast();
      break;
    }

      // Kicker Messages

    case CAN_R_KICK_ALIVE: {
      MIRO_DBG(SPARROW, LL_PRATTLE,
	       "Consumer::receiveThread:  received message: KICK_ALIVE");

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "KICK_ALIVE_VERSION: "<< versNr << "." << versSub);
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "KICK_TIME_STAMP (sec): " << message.shortData(2));
      Miro::Guard guard(kickerAliveMutex);
      kickerAliveCond.broadcast();
      break;
    }

      // Servo Messages

    case CAN_R_SERVO_ALIVE: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread:  received message: SERVO_ALIVE");

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "SERVO_ALIVE_VERSION: "<< versNr << "." << versSub);
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "SERVO_TIME_STAMP (sec): " << message.shortData(2));
      Miro::Guard guard(servoAliveMutex);
      servoAliveCond.broadcast();
      break;
    }

      // Infrared Messages:

    case CAN_R_IR_GET_CONT:
      MIRO_DBG(SPARROW, LL_PRATTLE,
	       "Consumer::receiveThread: receiveThread message: IR_CONT");

      if (pIR_) {
	Sparrow::Parameters * param = Sparrow::Parameters::instance(); //uli
	long calRange;
	Miro::RangeGroupEventIDL * data = new Miro::RangeGroupEventIDL();
	Miro::timeA2C(message.time(), data->time);
	data->group = 0;
	data->range.length(8);
	for (int i = 7; i >= 0; --i)  {
	  if (message.charData(i) != -1) {

	    calRange = ((int)message.charData(i) * 10 - param->irScaling[i].offset);
	    calRange = calRange - param->irScaling[i].minDistance;
	    calRange = (long) (calRange * param->irScaling[i].scaling);
	    calRange = calRange + param->irScaling[i].minDistance ;
	    if ((calRange < param->irScaling[i].minDistance) ||
		(calRange > param->irScaling[i].maxDistance)) {
	      calRange = -1;
	    }
	  }
	  else {
	    calRange = -1;
	  }
	  data->range[i] = calRange;
	}
	pIR_->integrateData(data);
      }
      break;

    case CAN_R_IR_ALIVE: {
      MIRO_DBG(SPARROW, LL_PRATTLE,
	       "Consumer::receiveThread:  received message: IR_ALIVE");

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "IR_ALIVE_VERSION: "<< versNr << "." << versSub);
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "IR_TIME_STAMP (sec): " << message.shortData(2));
      Miro::Guard guard(irAliveMutex);
      irAliveCond.broadcast();
      break;
    }
      // Debug Messages

    case CAN_R_DBG_PRINT:
      for (int k = 0; k < std::min(message.length(), 8); ++k)
	//if (message.canMessage()->d[k] != 0)
	//  cout << message.canMessage()->d[k] << flush;
      break;

    case CAN_R_DBG_ALIVE: {
      MIRO_DBG(SPARROW, LL_PRATTLE,
	       "Consumer::receiveThread:  received message: DBG_ALIVE");

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "DBG_ALIVE_VERSION: "<< versNr << "." << versSub);
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE,
		    "DBG_TIME_STAMP (sec): " << message.shortData(2));
      Miro::Guard guard(dbgAliveMutex);
      dbgAliveCond.broadcast();
      break;
    }

    default:
      MIRO_LOG_OSTR(LL_ERROR, 
		    "SparrowConsumer: Unhandled can bus message: " << message);
    }
  }
}
