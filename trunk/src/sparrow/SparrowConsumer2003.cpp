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



  Consumer2003::Consumer2003():
    params_(Parameters::instance())
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
				   PanTiltImpl * _panTilt,
				   AliveCollector * _aliveCollector)
  {
    connection_ = _connection;
    pOdometry_ = _pOdometry;
    pIR1_ = _pIR1;
    faulConsumer = _faulConsumer;
    panTilt_ = _panTilt;
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
      Miro::ScanDescriptionIDL_var description = pIR1_->getScanDescription();
      irValues_.resize(5);
      timeIndex_.resize(description->group.length());

      for(int k = 0; k < 5; k++){
	irValues_[k].resize(description->group.length());

	for(unsigned int i = 0; i < description->group.length(); i++){
	  irValues_[k][i].resize(description->group[i].sensor.length());
	  timeIndex_[i] = 0;
	  for(unsigned int j = 0; j < description->group[i].sensor.length(); j++){

	    irValues_[k][i][j] = -1;
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

    connection_->boardReply = 1;

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


    case CAN_R_PAN_ALIVE_2005: {
      MIRO_DBG(SPARROW, LL_PRATTLE, 
	       "Consumer::receivedThread:  received message: PAN2005_ALIVE");

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

	// Odometry messages...

    case CAN_MOTOR_TICKS_LEFT_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, "MotorController received  MotorTicksLeft2003!");
      FaulController::OdometryMessage 
	odoMessage(FaulController::OdometryMessage::LEFT);
      if (params_->module == "pcan") {
	odoMessage.setTicks(message.longData(0));
      }
      else {
	canmsg * msg_;
	message.canMessage((int **) &msg_);
	odoMessage.setTicks(*(reinterpret_cast<long *>(msg_->d)));
      }
      odoMessage.time() = message.time();

      if(faulConsumer)
	faulConsumer->handleMessage(&odoMessage);
      break;
    }

    case CAN_MOTOR_TICKS_RIGHT_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, "MotorController received  MotorTicksRight2003!");
      FaulController::OdometryMessage 
	odoMessage(FaulController::OdometryMessage::RIGHT);

      if(params_->module == "pcan") {
	odoMessage.setTicks(message.longData(0));
      }
      else{
	canmsg * msg_;
	message.canMessage((int **) &msg_);
	odoMessage.setTicks(*(reinterpret_cast<long *>(msg_->d)));
      }
      odoMessage.time() = message.time();

      if(faulConsumer)
	faulConsumer->handleMessage(&odoMessage);
      break;
    }
      
    case CAN_MOTOR_TICKS_2003: {
      MIRO_DBG(SPARROW, LL_PRATTLE, "MotorController received MotorTicks2003!");

      if(faulConsumer) {
	      
	FaulController::OdometryMessage 
	  odoLeft(FaulController::OdometryMessage::DELTA_LEFT);
	FaulController::OdometryMessage 
	  odoRight(FaulController::OdometryMessage::DELTA_RIGHT);
	
	MIRO_ASSERT(params_->module == "pcan");
	
	// get can message buffer
	//	canmsg * msg_;
	//	message.canMessage((int **) &msg_);
	
	// get left clock and ticks
	{
	  
	  unsigned int clockL = ACE_NTOHL(message.longData(0));//*(reinterpret_cast<long *>(msg_->d));
	  int deltaL = clockL & 0x00ffffff;
	  deltaL <<= 8;
	  deltaL >>= 8;
	  clockL >>= 24;
	  
	  odoLeft.setTicks(deltaL);
	  odoLeft.setClock((unsigned char) clockL);
	  odoLeft.time() = message.time();
      }
	
	// get right clock and ticks
	{
	  unsigned int clockR = ACE_NTOHL(message.longData(4)); //*(reinterpret_cast<long *>(msg_->d + 4));
	  int deltaR = clockR & 0x00ffffff;
	  deltaR <<= 8;
	  deltaR >>= 8;
	  clockR >>= 24;
	  
	  odoRight.setTicks(deltaR);
	  odoRight.setClock((unsigned char) clockR);
	  odoRight.time() = message.time();
	}
      
	faulConsumer->handleMessage(&odoLeft);
	faulConsumer->handleMessage(&odoRight);
      }
      break;
    }

      // Pan 2005 messages
    case CAN_R_PAN_TICKSPERDEG_2005:
      MIRO_DBG(SPARROW, LL_NOTICE, "SparrowConsumer2003: Received pan calibration message.");
      connection_->setPanTicksPerDegree(message.longData(0));
      break;

    case CAN_R_PAN_POSITION_2005:
      // TODO: store current pan position in PanTilt...
      break;
      
    case CAN_R_PAN_RESET_2005:
      MIRO_DBG(SPARROW, LL_NOTICE, "SparrowConsumer2003: Received pan reset message.");
      // register timer handler for pan calibration query
      connection_->deferredQueryPanTicksPerDegree(ACE_Time_Value(5));
      break;

    case CAN_PAN_ERROR_2005:
      {
	char const * err = NULL;
	switch(message.byteData(0))
	{
	case 0x01:	//ERR_UNKNOWN_COMMAND = 0x01;
	  err = "Unknown Command sent to Pan2005 controller";
	  break;
	case 0x02:	//ERR_REPORT_VERSION = 0x02;
	  err = "Pan2005 controller rebooted";
	  break;
	case 0x03:	//ERR_SER_RX_OVERFLOW = 0x03;  // serial rx buffer overflow
	  err = "RX buffer overflow on Pan2005 controller";
	  break;
	case 0x04:	//ERR_SER_TX_OVERFLOW = 0x04;  // serial tx buffer overflow
	  err = "TX buffer overflow on Pan2005 controller";
	  break;
	case 0x05:	//ERR_NOT_READY = 0x05;
	  connection_->deferredQueryPanTicksPerDegree(ACE_Time_Value(1));
	  err = "Pan2005 controller not ready for panning, still calibrating";
	  break;
	case 0x06:	//ERR_NODATA = 0x06;
	  err = "Pan2005 controller missing data for panning";
	  break;
	case 0x07:	//ERR_OUT_OF_BOUNDS = 0x07;
	  err = "Pan2005 reports out of panning range error";
	  break;
	case 0x08:	//ERR_NO_RESPONSE = 0x08;      // Debug Messages
	  err = "Pan2005 reports motorcontroller IO timeout";
	  break;
	default:
	  err = "Pan2005 unknown pan error.";
	}
	MIRO_LOG(LL_ERROR, err);
	break;
      }
    default:
      MIRO_LOG_OSTR(LL_WARNING, 
		    "SparrowConsumer2003: Unhandled can bus message: " << message);
    }
  }


  long
  Consumer2003::integrateIrValues(unsigned int group, unsigned int sensor, long value)
  {

    irValues_[timeIndex_[group]][group][sensor] = value;
    int counter = 0;
    long sum = 0;
    for (int i = 0; i < INTEGRATION_NUMBER; i++) {
      if (irValues_[i][group][sensor] == -1) {
	++counter;
      }
      sum += irValues_[i][group][sensor];
    }

    timeIndex_[group] = (timeIndex_[group] + 1) % INTEGRATION_NUMBER;

    if (counter >= 1) {
      return -1;
    }
    else {
      return (long) (sum/INTEGRATION_NUMBER);
    }
  }
}
