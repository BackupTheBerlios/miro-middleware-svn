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
    init_(2),
    xPos_(0.),
    yPos_(0.),
    ticksL_(0.),
    ticksR_(0.),
    prevTicksL_(0.),
    prevTicksR_(0.),
    wheelBase_(params_->motion.wheelBase)
  {
    DBG(cout << "Constructing FaulMotorConsumer." << endl);
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
      const FaulTty::OdometryMessage * pFaulMsg = 
	static_cast<const FaulTty::OdometryMessage *>(_message);

      if (pFaulMsg->wheel_ == FaulTty::OdometryMessage::LEFT) {
	ticksL_ = pFaulMsg->ticks_;
	timeStampL_ = pFaulMsg->time();
      }
      else {
	ticksR_ = pFaulMsg->ticks_;
	timeStampR_ = pFaulMsg->time();

	if (init_ == 0) {
	  double dL = -(ticksL_ + prevTicksL_) / params_->leftTicksPerMM;
	  double dR = (ticksR_ - prevTicksR_) / params_->rightTicksPerMM;

	  // reset tick counters to minimize overhead
	  //if ((pFaulMsg->lPos < 3000) or (pFaulMsg->lPos > 10000) pConnection->setPos1(0); zur zeit ohne zurücksetzung der pos
	  //if ((pFaulMsg->RPos < 3000) or (pFaulMsg->RPos > 10000) pConnection->setPos0(0);

	  // calculate new orientation
	  double turn = -(dL - dR) / wheelBase_;
	  status_.position.heading = (status_.position.heading + turn);
	  if (status_.position.heading <= -M_PI)
	    status_.position.heading += 2. * M_PI;
	  else if (status_.position.heading > M_PI)
	    status_.position.heading -= 2. * M_PI;

	  double dist = (dL + dR) / 2.;
	  xPos_ += cos(status_.position.heading) * dist;
	  yPos_ += sin(status_.position.heading) * dist; 

	  ACE_Time_Value timeDelta = timeStampR_ - prevTimeStampR_;

	  double deltaT = (double)timeDelta.sec() + (double)timeDelta.usec() / 1000000.;

	  // compose odometry message
	  Miro::timeA2C(timeStampR_, status_.time);
	  status_.position.point.x = (long) xPos_;
	  status_.position.point.y = (long) yPos_; 
	  status_.velocity.translation = (long)(dist * deltaT);
	  status_.velocity.rotation = turn * deltaT;

	  //cout << status_.time.sec<< "  " << status_.time.usec <<endl; //(dL+dR) / 2*dtime<<  endl;
	  
	  // save current values for next iteration
	  prevTimeStampL_ = timeStampL_;
	  prevTimeStampR_ = timeStampR_;
	  prevTicksL_ = ticksL_;
	  prevTicksR_ = ticksR_;

	  pOdometry_->integrateData(status_);
	  return;
	}
	else {
	  // save current values for next iteration
	  prevTimeStampL_ = timeStampL_;
	  prevTimeStampR_ = timeStampR_;
	  prevTicksL_ = ticksL_;
	  prevTicksR_ = ticksR_;

	  --init_;
	}
      }
    }
  }
};

