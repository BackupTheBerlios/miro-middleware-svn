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
#include "miro/RangeSensorImpl.h"
#include "miro/TimeHelper.h"
#include "can/Parameters.h"



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
		     Miro::RangeSensorImpl * _pIR) :
    connection(_connection),
    pOdometry_(_pOdometry),
    pIR_(_pIR),
    params_(Parameters::instance()),
    status_(),
    irAliveMutex1(),
    irAliveCond1(irAliveMutex1),
    irAliveMutex2(),
    irAliveCond2(irAliveMutex2)

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
    status_(),
    irAliveMutex1(),
    irAliveCond1(irAliveMutex1),
    irAliveMutex2(),
    irAliveCond2(irAliveMutex2)

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

    irAliveCond1.broadcast();

    irAliveMutex1.release();

    irAliveCond2.broadcast();

    irAliveMutex2.release();

  }

  void Consumer2003::registerInterfaces(Connection2003 * _connection,
	     				Miro::OdometryImpl * _pOdometry,
	     				Miro::RangeSensorImpl * _pIR)
  {



     connection = _connection;
     pOdometry_ = _pOdometry;
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
  Consumer2003::handleMessage(const Miro::DevMessage * _message)
  {
    const Can::Message& message = *((Can::Message*)_message);
    int tmp;
    int versNr, versSub;

    connection->boardReply = 1;

    Can::Parameters *CanParams = new Can::Parameters();

    CanId msgID = message.id();

    if(CanParams->module == "sja1000")
       msgID = (msgID | 0x80000000);

    switch (msgID) {

      case CAN_R_IR_GET_CONT1:
      DBG(cout << "Consumer::receiveThread: receiveThread message: IR_CONT1" << endl);

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

      case CAN_R_IR_GET_CONT2:
      DBG(cout << "Consumer::receiveThread: receiveThread message: IR_CONT2" << endl);

      if (pIR_) {
	Sparrow::Parameters * param = Sparrow::Parameters::instance(); //uli
	long calRange;
	Miro::RangeGroupEventIDL * data = new Miro::RangeGroupEventIDL();
	Miro::timeA2C(message.time(), data->time);
	data->group = 1;
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

    case CAN_R_IR_ALIVE1: {
      DBG(cout << "Consumer::receiveThread:  received message: IR_ALIVE1" << endl);

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      cout << "IR_ALIVE_VERSION: "<< versNr << "." << versSub << endl;
      cout << "IR_TIME_STAMP (sec):" << message.shortData(2) << endl;
      Miro::Guard guard(irAliveMutex1);
      irAliveCond1.broadcast();
      break;
    }

    case CAN_R_IR_ALIVE2: {
      DBG(cout << "Consumer::receiveThread:  received message: IR_ALIVE2" << endl);

      versSub = message.shortData(0);
      versNr = versSub >> 4;
      versSub = versSub & 0x0F; // versNr loeschen mit AND 00001111
      cout << "IR_ALIVE_VERSION: "<< versNr << "." << versSub << endl;
      cout << "IR_TIME_STAMP (sec):" << message.shortData(2) << endl;
      Miro::Guard guard(irAliveMutex2);
      irAliveCond2.broadcast();
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
