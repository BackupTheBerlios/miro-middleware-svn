// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "PioneerConsumer.h"
#include "PioneerStallImpl.h"
#include "PioneerParameters.h"

#include "miro/RangeEventC.h"
#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/TimeHelper.h"

#include "psos/PsosMessage.h"
#include <math.h>
// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using Psos::ServerInfoMessage;
using Miro::RangeBunchEventIDL;

namespace Pioneer
{
  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Miro::RangeSensorImpl * _pSonar,
		     Miro::OdometryImpl * _pOdometry,
		     Pioneer::StallImpl * _pStall) :
    pSonar(_pSonar),
    pOdometry(_pOdometry),
    pStall(_pStall),
    prevX(0),
    prevY(0),
    prevTheta(0.),
    params_(Parameters::instance())
  { 
    DBG(cout << "Constructing PioneerConsumer." << endl);

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
    DBG(cout << "Destructing PioneerConsumer." << endl);
  }

  // reads incoming packets from the psos connection and stores the values
  // in the local (class internal) variables.

  void 
  Consumer::handleMessage(const Miro::DevMessage * _message)
  {
    // DBG(cerr << "PioneerConsumer: handle message" << endl);

    const Psos::Message * pPsosMsg = static_cast<const Psos::Message *>(_message);
    
    switch (pPsosMsg->header()) {

    case 0xfafb: 
      {
	
	const ServerInfoMessage * message = static_cast<const ServerInfoMessage *>(_message);
	
	//------------------------------
	// evaluating odometry data
	//------------------------------

	if (pOdometry) {
	  short x, y, x0, y0, dX, dY;
	  double velL, velR, weelDist;
	  short bat, rBump, lBump;
	
	  weelDist = 320;  // nur zum test muss noch allgemein gemacht werden
	  // the odometry data is 15bit unsigned
	  // but we interret it as a signed value 
	  x = message->xPos();      // x position
	  x |= (x & 0x4000) << 1;  // sign extend 15th bit
	  x0 = x;                  // temporary
	  y = message->yPos();      // y position 
	  y |= (y & 0x4000) << 1;  // sign extend 15th bit
	  y0 = y;                  // temporary

	  // we have to cover under and overflows 
	  if (prevX < 0 && x > 0 && (x & 0x2000)) // underflow
	    x |= 0xb000;
	  else if (prevX > 0 && x < 0 && (prevX & 0x2000))  // overflow
	    prevX |= 0xb000;

	  if (prevY < 0 && y > 0 && (y & 0x2000)) // underflow
	    y |= 0xb000;
	  else if (prevY > 0 && y < 0 && (prevY & 0x2000))  // overflow
	    prevY |= 0xb000;

	  // calculate position delta
	  dX = x - prevX;
	  dY = y - prevY;

	  // save new position
	  prevX = x0;
	  prevY = y0;
      
	  // fill motion status data struct
	  Miro::timeA2C(message->time(), status_.time);
	  status_.position.point.x += (double)dX * params_->distConvFactor;
	  status_.position.point.y += (double)dY * params_->distConvFactor;

	  // these aren't calculated yet
	  velL = message->lVel() * params_->velConvFactor;
	  velR = message->rVel() * params_->velConvFactor;
	  status_.position.heading = message->theta() * params_->angleConvFactor;
	  status_.velocity.translation = (long) rint((velL + velR)/2);
	  status_.velocity.rotation = (velL - velR)*360/(2*M_PI*(-weelDist));

	  pOdometry->integrateData(status_);
     
	  rBump = message->rBumper();
	  lBump = message->lBumper();
	  bat = message->battery();
	  pStall->integrateData(rBump,lBump,bat);
	  //cout << "Stall: " << lBump << "--" << rBump << endl;
	  //cout << "Battery: " << bat << endl;
	}

	//------------------------------
	// evaluating sonar data
	//------------------------------

	if (pSonar && message->sonarReadings() > 0) {
	  //  DBG(cout << "Sonar readings: " << message->sonarReadings() << endl);

	  RangeBunchEventIDL * pSonarEvent = new RangeBunchEventIDL();
	  pSonarEvent->time.sec  = message->time().sec();
	  pSonarEvent->time.usec = message->time().usec();
	  pSonarEvent->sensor.length(message->sonarReadings());

	  // iterate through new data

	  for (int i = message->sonarReadings() - 1; i >= 0; --i) {
	    int group = 0;
	    int index = message->sonarNumber(i);
	
	    // peoplebot sonars
	    if (index >= 8) {
	      if (index < 16)
		++group;
	      index -= 8;
	    }

	    pSonarEvent->sensor[i].group = group;
	    pSonarEvent->sensor[i].index = index;
	    pSonarEvent->sensor[i].range = 
	      (CORBA::Long) ((double)message->sonarValue(i) * params_->rangeConvFactor);
	  }

	  //cout << "sonarReadings  " << message->sonarReadings() << endl;  
	  pSonar->integrateData(pSonarEvent);
	}

	//------------------------------
	// evaluating stall and bumper data
	//------------------------------

	cout << "Bumpers: ";
	unsigned short flags = message->flags();
	for (unsigned int i = 0; i < 16; ++i) {
	  cout << ((flags >> i) & 1) << " ";
	}
	cout << endl;
	
	break;
      }
    default:
      std::cerr << "Unkown message header: " << hex << pPsosMsg->header() << dec << endl;
    }
  }
};

