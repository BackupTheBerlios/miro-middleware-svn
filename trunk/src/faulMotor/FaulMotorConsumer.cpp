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
    prevPosL(0.),
    prevPosR(0.),
    xPos(0.),
    yPos(0.)
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
    if (pOdometry_) {
      double dL, dR;
      double posL, posR;
      float d; //radabstand

      const FaulTty::Message * pFaulMsg = static_cast<const FaulTty::Message *>(_message);

      posL = pFaulMsg->lPos;
      posR = pFaulMsg->rPos;
      d = 385.0;
      if (init == 0)
      {
	dL = 0.;
	dR = 0.;
	init = 1;
      }
      else {
	dL = -(posL / 112.) + prevPosL;
	dR = (posR / 112.) - prevPosR;
      }

      prevPosL = posL / 112.;
      prevPosR = posR / 112.;

      //if ((pFaulMsg->lPos < 3000) or (pFaulMsg->lPos > 10000) pConnection->setPos1(0); zur zeit ohne zurücksetzung der pos
      //if ((pFaulMsg->RPos < 3000) or (pFaulMsg->RPos > 10000) pConnection->setPos0(0);

      // calculate new orientation
      status_.position.heading =(status_.position.heading - (dL - dR) / d);
      if (status_.position.heading <= -M_PI)
	status_.position.heading += 2. * M_PI;
      else if (status_.position.heading > M_PI)
	status_.position.heading -= 2. * M_PI;

      xPos += cos(status_.position.heading) * (dL + dR) / 2.;
      yPos += sin(status_.position.heading) * (dL + dR) / 2.; 


      ACE_Time_Value timeDelta = pFaulMsg->time() - prevTimeStamp;

      double deltaT = (double)timeDelta.sec() + (double)timeDelta.usec() / 1000000.;

      // compose odometry message
      Miro::timeA2C(pFaulMsg->time(), status_.time);
      status_.position.point.x = (long) xPos;
      status_.position.point.y = (long) yPos; 
      status_.velocity.translation = (long)((dL + dR) / 2. * deltaT);
      status_.velocity.rotation = -(dL - dR) / d * deltaT;

      //cout << status_.time.sec<< "  " << status_.time.usec <<endl; //(dL+dR) / 2*dtime<<  endl;

      prevTimeStamp = pFaulMsg->time();

      pOdometry_->integrateData(status_);
    }
  }
};

