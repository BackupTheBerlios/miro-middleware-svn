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
  
#include "FaulMotorConsumer.h"
//#include "FaulMotorStallImpl.h"
#include "Parameters.h"

#include "FaulMotorMotionImpl.h"
//#include "faulMotor/CanonPanTiltImpl.h"
//#include "faulMotor/CanonPanTiltMessage.h"

#include "miro/RangeEventC.h"
#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"
#include "miro/TimeHelper.h"

#include "faulTty/FaulTtyMessage.h"
#include <cmath>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace FaulMotor
{
  //using Psos::ServerInfoMessage;
  using Miro::RangeBunchEventIDL;

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer( FaulMotor::MotionImpl * _pMotion,
		     Miro::OdometryImpl * _pOdometry):
  
           /*Miro::RangeSensorImpl * _pSonar,
		     Miro::RangeSensorImpl * _pTactile,
		     Miro::RangeSensorImpl * _pInfrared,
		     FaulMotor::MotionImpl * _pMotion,
		     Miro::OdometryImpl * _pOdometry,
		     Miro::BatteryImpl * _pBattery,
		     FaulMotor::StallImpl * _pStall,
		     Canon::CanonPanTiltImpl * _pCanonPanTilt) :*/
    //pSonar(_pSonar),
    //pTactile(_pTactile),
    //pInfrared(_pInfrared),
    pMotion(_pMotion),
    pOdometry(_pOdometry),
    //pBattery(_pBattery),
    //pStall(_pStall),
    //pCanonPanTilt(_pCanonPanTilt),
    prevX(0),
    prevY(0),
    //bumpers_(0x3e3e),
    params_(Parameters::instance())
  {
    DBG(cout << "Constructing FaulMotorConsumer." << endl);

    status_.position.point.x = 0.;
    status_.position.point.y = 0.;
    status_.position.heading = 0.;
    status_.velocity.translation = 0;
    status_.velocity.rotation = 0.;
    //pAnswer=pCanonPanTilt->getAnswer();
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//
  Consumer::~Consumer()
  { 
    DBG(cout << "Destructing FaulMotorConsumer." << endl);
  }

  // reads incoming packets from the psos connection and stores the values
  // in the local (class internal) variables.

  void 
  Consumer::handleMessage(const Miro::DevMessage * _message)
  {
      	long dL, dR;

      //cout << "hier ist der FaulMotorConsumer  : " << _message <<endl;
     /* berechnung der Daten aus den Ticks der motoren:
        drehwinkel:(x-y)/(2*pi*d)*360  	x grösserer radius
        						y kleinerer Radius
        						d Radabstand
        Radius: r=(y*360)/(winkel*2*pi)
        egozentrische x y position (begin nach vorne schauend)
        x-pos (r+d/2)*cos(winkel) - (r+d/2)
        y-pos (r+d/2)*sin(winkel)

         status_.position.point.x = 0.;
         status_.position.point.y = 0.;
         status_.position.heading = 0.;
         status_.velocity.translation = 0;
         status_.velocity.rotation = 0.;
        */

        const FaulTty::Message * pFaulMsg = static_cast<const FaulTty::Message *>(_message);

	dL = prevPosL-(pFaulMsg->lPos);
	dR = prevPosR-(pFaulMsg->rPos);
	prevPosL = pFaulMsg->lPos;
	prevPosR = pFaulMsg->rPos;

	//dieser fall nur x>y x,y>=0  x=dl  y=dR
	//status_.position.heading = 
//	cout <<prevPosR<< endl;
	/*if (dL==dL) {
		status_.position.heading = (dL-dR)/38;} 
		else
		{status_.position.heading=1;}	
*/
//status_.position.heading = (dL-dR)/(2*M_PI*385)*360; 
         status_.position.point.x = 0.;
         status_.position.point.y = 0.;
         //status_.position.heading = 0.;
         status_.velocity.translation = 0;
         status_.velocity.rotation = 0.;


	//cout <<"consumer gekommt: " <<pFaulMsg->lPos << endl;
      
      //pOdometry->integrateData(status_);
   

      
      
      
      
      
      
      
       // DBG(cerr << "FaulMotorConsumer: handle message" << endl);

    /*const Psos::Message * pPsosMsg = static_cast<const Psos::Message *>(_message);

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
	    pMotion->lr2velocity((CORBA::Long)velL, (CORBA::Long)velR, status_.velocity);*/
	  //pOdometry->integrateData(status_);
/*
// cout << velL << "\t" << velR << "\t" << Miro::rad2Deg(status_.velocity.rotation) << endl;
	}

	*/
  }
};

