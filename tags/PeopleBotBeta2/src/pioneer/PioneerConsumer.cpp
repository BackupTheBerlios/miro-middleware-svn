// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PioneerConsumer.h"
#include "PioneerStallImpl.h"
#include "Parameters.h"

#include "PioneerMotionImpl.h"
#include "pioneer/CanonPanTiltImpl.h"
//#include "pioneer/CanonPanTiltMessage.h"

#include "miro/RangeEventC.h"
#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"
#include "miro/TimeHelper.h"

#include "psos/PsosMessage.h"
#include <cmath>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Pioneer
{
  using Psos::ServerInfoMessage;
  using Miro::RangeBunchEventIDL;

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Miro::RangeSensorImpl * _pSonar,
		     Miro::RangeSensorImpl * _pTactile,
		     Miro::RangeSensorImpl * _pInfrared,
		     Pioneer::MotionImpl * _pMotion,
		     Miro::OdometryImpl * _pOdometry,
		     Miro::BatteryImpl * _pBattery,
		     Pioneer::StallImpl * _pStall,
		     Canon::CanonPanTiltImpl * _pCanonPanTilt) :
    pSonar(_pSonar),
    pTactile(_pTactile),
    pInfrared(_pInfrared),
    pMotion(_pMotion),
    pOdometry(_pOdometry),
    pBattery(_pBattery),
    pStall(_pStall),
    pCanonPanTilt(_pCanonPanTilt),
    prevX(0),
    prevY(0),
    bumpers_(0x3e3e),
    params_(Parameters::instance())
  { 
    DBG(cout << "Constructing PioneerConsumer." << endl);

    status_.position.point.x = 0.;
    status_.position.point.y = 0.;
    status_.position.heading = 0.;
    status_.velocity.translation = 0;
    status_.velocity.rotation = 0.;
    pAnswer=pCanonPanTilt->getAnswer();
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

    if (pPsosMsg->header() == 0xfbfa) { // standart header

      if ((pPsosMsg->id() & 0xFC) == 0x30) { //standard SIP
	
	const ServerInfoMessage * message = static_cast<const ServerInfoMessage *>(_message);
	//------------------------------
	// evaluating odometry data
	//------------------------------
	
	if (pOdometry) {
	  short x, y, x0, y0, dX, dY;
	  double velL, velR;
	
	  // the odometry data is 15bit unsigned
	  // but we interpret it as a signed value 
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
          if (status_.position.heading > M_PI)
            status_.position.heading -= 2 * M_PI;
	  if (pMotion)
	    pMotion->lr2velocity((CORBA::Long)velL, (CORBA::Long)velR, status_.velocity);
	  pOdometry->integrateData(status_);   

// cout << velL << "\t" << velR << "\t" << Miro::rad2Deg(status_.velocity.rotation) << endl;
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
	  
	if (pStall) {
	  unsigned short rBump = message->rBumper();
	  unsigned short lBump = message->lBumper();
	  pStall->integrateData(rBump,lBump);
	  //cout << "Stall: " << lBump << "--" << rBump << endl;
	}
	  
	if (pTactile) {
	  RangeBunchEventIDL * pTactileEvent = NULL;
	    
	  unsigned long counter = 0;
	  unsigned long index = 0;
	  unsigned long bumpers = message->bumpers();
	  unsigned long mask = (bumpers_ ^ bumpers) & 0x3e3e;
	    
	  if (mask) {
	    pTactileEvent = new RangeBunchEventIDL();
	    pTactileEvent->sensor.length(10);
	      
	    for (unsigned int i = 9; i < 14; ++i, ++index) {
	      if (mask & (1 << i)) {
		pTactileEvent->sensor[counter].group = 0;
		pTactileEvent->sensor[counter].index = index;
		pTactileEvent->sensor[counter].range = 
		  (bumpers & (1 << i))? 0 : -1; // Miro::RangeSensor::HORIZON_RANGE;
		  
		++counter;
	      }
	    }
	    for (unsigned int i = 1; i < 6; ++i, ++index) {
	      if (mask & (1 << i)) {
		pTactileEvent->sensor[counter].group = 0;
		pTactileEvent->sensor[counter].index = index;
		pTactileEvent->sensor[counter].range = 
		  (bumpers & (1 << i))? 0 : -1; //Miro::RangeSensor::HORIZON_RANGE;
		  
		++counter;
	      }
	    }
	      
	    Miro::timeA2C(message->time(), pTactileEvent->time);
	    pTactileEvent->sensor.length(counter);
	    pTactile->integrateData(pTactileEvent);
	  }
	  bumpers_ = bumpers;
	}
	  
	//-------------------------------------------------------------------
	// evaluating battery
	//-------------------------------------------------------------------
	if (pBattery) {
	  pBattery->integrateData(message->battery());
	  //cout << "Battery: " << bat << endl;
	}
      }

      //-------------------------------------------------------------------
      // SERAUXpac SIP
      //-------------------------------------------------------------------
      else if (pPsosMsg->id() == 0xb0) { //SERAUXpac SIP
	cout << "serial answer" << endl;
	if (pAnswer) {
	  Miro::Guard guard(pAnswer->mutex);
	  for (int i = 4; i < 4 + pPsosMsg->buffer()[2] - 3; ++i) 
	    pAnswer->add(pPsosMsg->buffer()[i]);
	  pAnswer->cond.broadcast();
	}
      }

      //-------------------------------------------------------------------
      // IOpac SIP
      //-------------------------------------------------------------------
      else if (pPsosMsg->id() == 0xf0) { //OPpac SIP
	if (pInfrared) {
	  Psos::IOpacMessage const * const message =
	    static_cast<Psos::IOpacMessage const * const>(_message);

	  int ir = message->ir() & 0x0f; // lower 4 bit represent the ir sensors of the ppb
	  if (ir != infrared_) {

	    Miro::RangeGroupEventIDL * pInfraredEvent = new Miro::RangeGroupEventIDL();
	    pInfraredEvent->time.sec  = message->time().sec();
	    pInfraredEvent->time.usec = message->time().usec();
	    pInfraredEvent->group = 0;
	    pInfraredEvent->range.length(4);

	    for (unsigned int i = 0; i < 4; ++i) {
	      pInfraredEvent->range[i] = -((ir >> i) & 1);
	    }
	    infrared_ = ir;

	    pInfrared->integrateData(pInfraredEvent);
	  }
	}
      }
      else {
       std::cerr << "Unkown message header: "
		 << hex << pPsosMsg->header() << dec << endl;
      }
    }
    else {
      std::cerr << "Unkown message header: "
		<< hex << pPsosMsg->header() << dec << endl;
    }
  }
};
