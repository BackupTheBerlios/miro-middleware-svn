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
  
#include "FaulMotorConsumer.h"
#include "Parameters.h"

#include "faulTty/FaulTtyMessage.h"

#include "miro/OdometryImpl.h"
#include "miro/TimeHelper.h"

#include <cmath>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace FaulMotor
{

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Miro::OdometryImpl * _pOdometry):
    params_(Parameters::instance()),
    pOdometry_(_pOdometry),
    prevX(0),
    prevY(0),
    prevPosL(0),
    prevPosR(0),
    prevSec(0),
    prevUsec(0)
  {
    DBG(cout << "Constructing FaulMotorConsumer." << endl);
    init = 0;
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
    DBG(cout << "Destructing FaulMotorConsumer." << endl);
  }

  // reads incoming packets from the faulhaber connection and stores the values
  // in the local (class internal) variables.

  void
  Consumer::handleMessage(const Miro::DevMessage * _message)
  {
      	long dL, dR;
	long posL, posR, dtime;
	float d; //radabstand
	float r, egoX, egoY;
	float test;

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

	posL = pFaulMsg->lPos;
	posR = pFaulMsg->rPos;
	d = 385.0;
        if (init ==0 )
	{
		dL = 0;
		dR = 0;
		init =1;
	}else {
		dL = -(posL/112)+prevPosL;
		dR = (posR/112)-prevPosR;

	}

	prevPosL = posL/112;
	prevPosR = posR/112;

	//if ((pFaulMsg->lPos < 3000) or (pFaulMsg->lPos > 10000) pConnection->setPos1(0); zur zeit ohne zurücksetzung der pos
	//if ((pFaulMsg->RPos < 3000) or (pFaulMsg->RPos > 10000) pConnection->setPos0(0);

	status_.position.heading =(status_.position.heading -(dL-dR)/d);
	if (status_.position.heading <= -M_PI)
	  status_.position.heading += 2. * M_PI;
	else if (status_.position.heading > M_PI)
	  status_.position.heading -= 2. * M_PI;

	//status_.position.heading =test;
	//if (test>=M_PI) status_.position.heading =test-M_PI;
	//if (test<-M_PI) status_.position.heading =test+M_PI;
	//test=-(dL-dR)/(2*M_PI*385)*360;

        //cout << "posL: " << prevPosL << " PosR: " << prevPosR ;
	//cout << "  dL: " << dL << " dR: " << dR << " heading in °" << test << endl;

/*
	r =d;
	if ((dL<dR)and(dL!=0)) {
		r=(dL*360)/(test*2*M_PI);
	}else{
		if ((dL>dR) and (dR != 0)) {
			 r=(dR*360)/(test*2*M_PI);
			 }
		}
	if (dL == dR)
	{  r =0;
	   egoX = 0;
	   egoY = dL;
	   status_.position.point.y =status_.position.point.y +dL;
	   //cout << " gerade dL: " << dL<<"  piontY" << status_.position.point.y<<endl;

	}else{
	   //egoX = (r+d/2)-(r+d/2)*cos((dL-dR)/d);
           egoY = (r+d/2)*sin(-(dL-dR)/d);
	   status_.position.point.y =status_.position.point.y +egoY;
	//cout << "r: " << r << " egoY: " << egoY<< "   "  << test << "  dL: " << dL << " dR: " << dR<<endl;
	}
	egoX = (r+d/2)-(r+d/2)*cos((dL-dR)/d);
	status_.position.point.x =status_.position.point.x +egoX;

	//cout << "r: " << r << "  egoX: " << egoX << " egoY: " << egoY<< " winkel bogen "<< status_.position.heading<<endl;

	*/
	status_.position.point.x = status_.position.point.x - sin(status_.position.heading) * (dL+dR) / 2;
	status_.position.point.y = status_.position.point.y + cos(status_.position.heading) * (dL+dR) / 2; 
	//status_.position.point.x = 0.;
        //status_.position.point.y = 0.;
        //status_.velocity.translation = 0;
        //status_.velocity.rotation = 0.;
	Miro::timeA2C(pFaulMsg->time(), status_.time);
	if (prevUsec <= status_.time.usec) {
			dtime = status_.time.usec - prevUsec;
		}else{
			dtime = 1000000+status_.time.usec - prevUsec;}
	status_.velocity.translation = (dL+dR) / 2*dtime /100000;
	status_.velocity.rotation =-(dL-dR)/d *dtime /100000;
	//cout << status_.time.sec<< "  " << status_.time.usec <<endl; //(dL+dR) / 2*dtime<<  endl;
	prevSec = status_.time.sec;
	prevUsec = status_.time.usec;

      if (pOdometry_)
        pOdometry_->integrateData(status_);







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

