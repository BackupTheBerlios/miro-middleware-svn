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


#include "miro/ButtonsC.h"

#include "SparrowConsumer2003.h"
#include "SparrowConnection2003.h"
#include "IRLookupTable.h"
#include "miro/RangeSensorImpl.h"
#include "miro/TimeHelper.h"
#include "can/Parameters.h"
#include "faulTty/FaulTtyMessage.h"



#include "can/CanMessage.h"

#include <miro/Exception.h>
#include <algorithm>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Sparrow
{

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer2003::Consumer2003(Connection2003 * _connection,
		     Miro::OdometryImpl * _pOdometry,
		     Miro::RangeSensorImpl * _pIR1,
		     Miro::RangeSensorImpl * _pIR2) :
    connection(_connection),
    pOdometry_(_pOdometry),
    pIR1_(_pIR1),
    pIR2_(_pIR2),
    params_(Parameters::instance()),
    status_()

/*
    irAliveMutex1(),
    irAliveCond1(irAliveMutex1),
    irAliveMutex2(),
    irAliveCond2(irAliveMutex2)
*/
    //x_(0.),
    //y_(0.),
    //index_(0)
  {
    cout << "Constructing SparrowConsumer." << endl;

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

  Consumer2003::Consumer2003():
    params_(Parameters::instance()),
    status_()

/*
    irAliveMutex1(),
    irAliveCond1(irAliveMutex1),
    irAliveMutex2(),
    irAliveCond2(irAliveMutex2)
*/

    //x_(0.),
    //y_(0.),
    //index_(0)
  {
    cout << "Constructing SparrowConsumer." << endl;

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
    cout << "Destructing SparrowConsumer." << endl;
/*
    irAliveCond1.broadcast();

    irAliveMutex1.release();

    irAliveCond2.broadcast();

    irAliveMutex2.release();
*/
  }

  void Consumer2003::registerInterfaces(Connection2003 * _connection,
	     				Miro::OdometryImpl * _pOdometry,
	     				Miro::RangeSensorImpl * _pIR1,
					Miro::RangeSensorImpl * _pIR2,
					FaulMotor::Consumer * _faulConsumer,
					AliveCollector * _aliveCollector)
  {



     connection = _connection;
     pOdometry_ = _pOdometry;
     pIR1_ = _pIR1;
     pIR2_ = _pIR2;
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







  }

  // reads incoming packets from the canBus connection and stores the values
  // in the local (class internal) variables.
  void
  Consumer2003::handleMessage(const Miro::DevMessage * _message)
  {
    Can::Message& message = *((Can::Message*)_message);
    int tmp;
    int versNr, versSub;

    connection->boardReply = 1;

    Can::Parameters *CanParams = new Can::Parameters();

    CanId msgID = message.id();

    if(CanParams->module == "pcan")
       msgID = (msgID | 0x80000000);

    switch (msgID) {

      case CAN_R_IR_GET_CONT1:{
      DBG(cout << "Consumer::receiveThread: receiveThread message: IR_CONT1" << endl);
      if (pIR1_) {
	Sparrow::Parameters * param = Sparrow::Parameters::instance(); //uli
	long calRange;
	Miro::RangeGroupEventIDL * data = new Miro::RangeGroupEventIDL();
	Miro::timeA2C(message.time(), data->time);
	data->group = 0;
	data->range.length(8);
	for (int i = 7; i >= 0; --i)  {
        calRange = IR_LOOKUP_TABLE[message.byteData(i)];

        // -1 wenn Entfernung >= 50cm
	if (calRange == 500) {
	  calRange = -1;
	}

	  data->range[i] = calRange;
	}
	pIR1_->integrateData(data);
      }

      break;
      }

      case CAN_IR_GET_CONT2:{
      DBG(cout << "Consumer::receiveThread: receiveThread message: IR_CONT2" << endl);

      if (pIR2_) {
	Sparrow::Parameters * param = Sparrow::Parameters::instance(); //uli
	long calRange;
	Miro::RangeGroupEventIDL * data = new Miro::RangeGroupEventIDL();
	Miro::timeA2C(message.time(), data->time);
	data->group = 1;
	data->range.length(8);
	for (int i = 7; i >= 0; --i)  {
	/*  if (message.charData(i) != -1) {


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
	  } */

	  calRange = IR_LOOKUP_TABLE[message.byteData(i)];

        // -1 wenn Entfernung >= 50cm
	if (calRange == 500) {
	  calRange = -1;
	}
	  data->range[i] = calRange;
	}
	pIR2_->integrateData(data);
      }
      break;
      }

// Kicker_Alive, Pan_Alive, Motor_Alive, IR_Alive1+2

    case CAN_R_IR_ALIVE1:{
      DBG(cout << "Consumer::receiveThread:  received message: IR_ALIVE1" << endl);
      //pAliveCollector->setLastInfraredAlive(ACE_OS::gettimeofday());

      break;
    }


    case CAN_IR_ALIVE2:{
      DBG(cout << "Consumer::receiveThread:  received message: IR_ALIVE2" << endl);
      //pAliveCollector->setLastInfraredAlive(ACE_OS::gettimeofday());

      break;
    }


    case CAN_R_MOTOR_ALIVE:{
      DBG(cout << "Consumer::receiveThread:  received message: MOTOR_ALIVE" << endl);
      //pAliveCollector->setLastMotorAlive(ACE_OS::gettimeofday());

      break;
    }


    case CAN_PAN_ALIVE:{
      DBG(cout << "Consumer::receiveThread:  received message: PAN_ALIVE" << endl);
      //pAliveCollector->setLastPanAlive(ACE_OS::gettimeofday());

      break;
    }


    case CAN_R_KICK_ALIVE:{
      DBG(cout << "Consumer::receiveThread:  received message: KICK_ALIVE" << endl);
      //pAliveCollector->setLastKickAlive(ACE_OS::gettimeofday());

      break;
    }

    case CAN_MOTOR_TICKS_LEFT:{
      FaulTty::OdometryMessage odoMessage(FaulTty::OdometryMessage::LEFT);
      if(CanParams->module == "pcan"){
         Can::pcanmsg * msgp_;
	 message.canMessage(&msgp_);
         memcpy((void *) &(odoMessage.ticks_),(void *) (msgp_->Msg.DATA),  4);
      }
      else{
         canmsg * msg_;
	 message.canMessage(&msg_);
	 memcpy((void *) &(odoMessage.ticks_),(void *) (msg_->d),  4);
      }
      faulConsumer->handleMessage(&odoMessage);
      std::cout << "MotorTicksLeft" << message << endl;


       break;
    }

    case CAN_MOTOR_TICKS_RIGHT:{

       FaulTty::OdometryMessage odoMessage2(FaulTty::OdometryMessage::RIGHT);
       if(CanParams->module == "pcan"){
         Can::pcanmsg * msgp_;
	 message.canMessage(&msgp_);
         memcpy((void *) &(odoMessage2.ticks_),(void *) (msgp_->Msg.DATA),  4);
      }
      else{
         canmsg * msg_;
	 message.canMessage(&msg_);
	 memcpy((void *) &(odoMessage2.ticks_),(void *) (msg_->d),  4);
      }
       faulConsumer->handleMessage(&odoMessage2);
       std::cout << "MotorTicksRight" << message << endl;
       break;

   }



      // Debug Messages



    default:
      cerr << "SparrowConsumer: Unhandled can bus message: "
	   << message << endl;
    }
  }

  /*short *
  Consumer::getTable1()
  {
    return table1;
  }

  short *
  Consumer::getTable2()
  {
    return table2;
  }*/

};
