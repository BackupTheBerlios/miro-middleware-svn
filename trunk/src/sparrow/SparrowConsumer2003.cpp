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


#include "idl/ButtonsC.h"

#include "SparrowConsumer2003.h"
#include "SparrowConnection2003.h"
#include "IRLookupTable.h"
#include "miro/RangeSensorImpl.h"
#include "miro/TimeHelper.h"
#include "can/Parameters.h"
#include "faulTty/FaulTtyMessage.h"

#include "can/CanMessage.h"
#include "can/pcan.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include <algorithm>

namespace Sparrow
{
  //------------------------//
  //----- constructors -----//
  //------------------------//
  const int Consumer2003::INTEGRATION_NUMBER = 3;


  Consumer2003::Consumer2003(Connection2003 * _connection,
			     Miro::OdometryImpl * _pOdometry,
			     Miro::RangeSensorImpl * _pIR1) :
    connection(_connection),
    pOdometry_(_pOdometry),
    pIR1_(_pIR1),
    IrValues(),
    description_(),
    params_(Parameters::instance()),
    status_()
  {
    MIRO_LOG_CTOR("Sparrow::Consumer2003");

    pAliveCollector = NULL;
    faulConsumer = NULL;
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

    if(pIR1_){
      description_ = pIR1_->getScanDescription();
      IrValues.resize(5);
      TimeIndex.resize(description_->group.length());

      for(int k = 0; k < 5; k++){
	IrValues[k].resize(description_->group.length());


	for(unsigned int i = 0; i < description_->group.length(); i++){
	  IrValues[k][i].resize(description_->group[i].sensor.length());
	  TimeIndex[i] = 0;
	  for(unsigned int j = 0; j < description_->group[i].sensor.length(); j++){

	    IrValues[k][i][j] = -1;
	  }
	}
      }
    }
  }

  Consumer2003::Consumer2003():
    IrValues(),
    description_(),
    params_(Parameters::instance()),
    status_()
  {
    MIRO_LOG_CTOR("Sparrow::Consumer2003");

    status_.position.point.x = 0.;
    status_.position.point.y = 0.;
    status_.position.heading = 0.;
    status_.velocity.translation = 0;
    status_.velocity.rotation = 0.;
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//
  Consumer2003::~Consumer2003()
  {
    MIRO_LOG_DTOR("Sparrow::Consumer2003");
  }

  void 
  Consumer2003::registerInterfaces(Connection2003 * _connection,
				   Miro::OdometryImpl * _pOdometry,
				   Miro::RangeSensorImpl * _pIR1,
				   FaulMotor::Consumer * _faulConsumer,
				   AliveCollector * _aliveCollector)
  {
    connection = _connection;
    pOdometry_ = _pOdometry;
    pIR1_ = _pIR1;
    faulConsumer = _faulConsumer;
    pAliveCollector = _aliveCollector;

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

    if(pIR1_){
      description_ = pIR1_->getScanDescription();

      IrValues.resize(5);
      TimeIndex.resize(description_->group.length());

      for(int k = 0; k < 5; k++){
	IrValues[k].resize(description_->group.length());

	for(unsigned int i = 0; i < description_->group.length(); i++){
	  IrValues[k][i].resize(description_->group[i].sensor.length());
	  TimeIndex[i] = 0;
	  for(unsigned int j = 0; j < description_->group[i].sensor.length(); j++){

	    IrValues[k][i][j] = -1;
	  }
	}
      }
    }
  }

  // reads incoming packets from the canBus connection and stores the values
  // in the local (class internal) variables.
  void
  Consumer2003::handleMessage(const Miro::DevMessage * _message)
  {
    Can::Message& message = *((Can::Message*)_message);
    // int tmp;
    // int versNr, versSub;

    connection->boardReply = 1;

    //Can::Parameters *CanParams = new Can::Parameters();

    CanId msgID = message.id();

    if(params_->module == "pcan")
      msgID = (msgID | 0x80000000);

    switch (msgID) {

    case CAN_R_IR_GET_CONT1_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread: receiveThread message: IR_CONT1");

      if (pIR1_) {

	Sparrow::Parameters * param = Sparrow::Parameters::instance(); //uli
	long calRange;
	Miro::RangeGroupEventIDL * data = new Miro::RangeGroupEventIDL();
	Miro::timeA2C(message.time(), data->time);
	data->group = 0;
	data->range.length(8);

	for (int i = 7; i >= 0; --i) {

	  calRange = IR_LOOKUP_TABLE[message.byteData(i)];
	  calRange = (calRange - param->irScaling[i].offset);
	  calRange = calRange - param->irScaling[i].minDistance;
	  calRange = (long) (calRange * param->irScaling[i].scaling);
	  calRange = calRange + param->irScaling[i].minDistance ;
	  if (calRange < param->irScaling[i].minDistance)
	    calRange = param->irScaling[i].minDistance;
	  if(calRange > param->irScaling[i].maxDistance)
	    calRange = -1;

	  data->range[i] = integrateIrValues(0, i, calRange);
	}
	pIR1_->integrateData(data);
      }

      break;
    }

    case CAN_R_IR_GET_CONT2_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread: receiveThread message: IR_CONT2");

      if (pIR1_) {
	Sparrow::Parameters * param = Sparrow::Parameters::instance(); //uli
	long calRange;
	Miro::RangeGroupEventIDL * data = new Miro::RangeGroupEventIDL();
	Miro::timeA2C(message.time(), data->time);
	data->group = 1;
	data->range.length(8);

	for (int i = 7; i >= 0; --i) {

	  calRange = IR_LOOKUP_TABLE[message.byteData(i)];
	  calRange = (calRange - param->irScaling[8+i].offset);
	  calRange = calRange - param->irScaling[8+i].minDistance;
	  calRange = (long) (calRange * param->irScaling[8+i].scaling);
	  calRange = calRange + param->irScaling[8+i].minDistance ;
	  if (calRange < param->irScaling[8+i].minDistance)
	    calRange = param->irScaling[8+i].minDistance;
	  if(calRange > param->irScaling[8+i].maxDistance)
	    calRange = -1;

	  data->range[i] = integrateIrValues(1, i, calRange);
	}
	pIR1_->integrateData(data);
      }
      break;
    }

      // Kicker_Alive, Pan_Alive, Motor_Alive, IR_Alive1+2

    case CAN_R_IR_ALIVE1_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	      "Consumer::receiveThread:  received message: IR_ALIVE1");

      if(pAliveCollector)
	pAliveCollector->setLastInfrared1Alive(ACE_OS::gettimeofday());
      break;
    }


    case CAN_R_IR_ALIVE2_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread:  received message: IR_ALIVE2");

      if(pAliveCollector)
	pAliveCollector->setLastInfrared2Alive(ACE_OS::gettimeofday());
      break;
    }


    case CAN_R_MOTOR_ALIVE_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread:  received message: MOTOR_ALIVE");

      if(pAliveCollector)
	pAliveCollector->setLastMotorAlive(ACE_OS::gettimeofday());
      break;
    }


    case CAN_R_PAN_ALIVE_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receivedThread:  received message: PAN_ALIVE");

      if(pAliveCollector)
	pAliveCollector->setLastPanAlive(ACE_OS::gettimeofday());
      break;
    }

    case CAN_R_KICK_ALIVE_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receiveThread:  received message: KICK_ALIVE");

      if(pAliveCollector)
	pAliveCollector->setLastKickAlive(ACE_OS::gettimeofday());
      break;
    }

    case CAN_MOTOR_CANN: {
      MIRO_DBG_OSTR(SPARROW, LL_PRATTLE, 
		    "MotorController received " << message.shortData(0) << 
		    " CanFloodPing-Messages!");
      break;
    }

    case CAN_MOTOR_TICKS_LEFT_2003: {
      FaulController::OdometryMessage 
	odoMessage(FaulController::OdometryMessage::LEFT);
      if (params_->module == "pcan") {
	pcanmsg * msgp_;
	message.canMessage((int **) &msgp_);
	odoMessage.ticks_ = message.longData(0);
      }
      else {
	canmsg * msg_;
	message.canMessage((int **) &msg_);
	memcpy((void *) &(odoMessage.ticks_),(void *) (msg_->d),  4);
      }
      odoMessage.setTimestamp(ACE_OS::gettimeofday());

      if(faulConsumer)
	faulConsumer->handleMessage(&odoMessage);
      break;
    }

    case CAN_MOTOR_TICKS_RIGHT_2003: {

      FaulController::OdometryMessage 
	odoMessage2(FaulController::OdometryMessage::RIGHT);

      if(params_->module == "pcan") {
	pcanmsg * msgp_;
	message.canMessage((int **) &msgp_);
	odoMessage2.ticks_ = message.longData(0);
      }
      else{
	canmsg * msg_;
	message.canMessage((int **) &msg_);
	memcpy((void *) &(odoMessage2.ticks_),(void *) (msg_->d),  4);
      }
      odoMessage2.setTimestamp(ACE_OS::gettimeofday());

      if(faulConsumer)
	faulConsumer->handleMessage(&odoMessage2);
      break;
    }

      // Debug Messages
    default:
      MIRO_LOG_OSTR(LL_WARNING, 
		    "SparrowConsumer2003: Unhandled can bus message: " << message);
    }
  }


  long
  Consumer2003::integrateIrValues(unsigned int group, unsigned int sensor, long value)
  {

    IrValues[TimeIndex[group]][group][sensor] = value;
    int counter = 0;
    long sum = 0;
    for (int i = 0; i < INTEGRATION_NUMBER; i++) {
      if (IrValues[i][group][sensor] == -1) {
	++counter;
      }
      sum += IrValues[i][group][sensor];
    }

    TimeIndex[group] = (TimeIndex[group] + 1) % INTEGRATION_NUMBER;

    if (counter >= 1) {
      return -1;
    }
    else {
      return (long) (sum/INTEGRATION_NUMBER);
    }
  }
}
