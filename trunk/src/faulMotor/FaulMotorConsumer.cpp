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
#include "FaulMotorConnection.h"
#include "Parameters.h"

#include "faulTty/FaulTtyMessage.h"
#include "sparrow/Parameters.h"

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
  using std::cout;
  using std::cerr;
  using std::endl;

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
    counterL(49),
    counterR(49),
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

  double deltaTR[50];
  double deltaTL[50];
  double deltaTLR[50];
  int counter = 49;

  void
  Consumer::handleMessage(const Miro::DevMessage * _message)
  {
    ++Connection::gotTicks_;

    if (pOdometry_) {

      const FaulController::OdometryMessage * pFaulMsg =
	static_cast<const FaulController::OdometryMessage *>(_message);

      if (pFaulMsg->wheel_ == FaulController::OdometryMessage::LEFT) {
//	cout << "L: " << pFaulMsg->ticks_;
	ticksL_ = pFaulMsg->ticks_;
	timeStampL_ = pFaulMsg->time();
      }
      else {
//	cout << "R: " << pFaulMsg->ticks_;
	ticksR_ = pFaulMsg->ticks_;
	timeStampR_ = pFaulMsg->time();
      }

      if (prevTimeStampL_+ ACE_Time_Value(0, 10000) < timeStampR_ &&
	  prevTimeStampR_+ ACE_Time_Value(0, 10000) < timeStampL_ &&
	  prevTimeStampL_ != timeStampL_ &&
	  prevTimeStampR_ != timeStampR_) {
	if (init_ == 0) {
	  double dL = -(ticksL_ - prevTicksL_) / params_->leftTicksPerMM;
	  double dR = (ticksR_ - prevTicksR_) / params_->rightTicksPerMM;

//	  cout << "dL: " << dL << " dR: " << dR << endl;

	  // reset tick counters to minimize overhead
	  //if ((pFaulMsg->lPos < 3000) or (pFaulMsg->lPos > 10000) pConnection->setPos1(0);
	  //if ((pFaulMsg->RPos < 3000) or (pFaulMsg->RPos > 10000) pConnection->setPos0(0);

	  // calculate new orientation
	  double turn = (dR - dL) / wheelBase_;
	  status_.position.heading += turn;

	  Miro::Angle::normalize(status_.position.heading);

	  double dist = (dL + dR) / 2.;
	  xPos_ += cos(status_.position.heading) * dist;
	  yPos_ += sin(status_.position.heading) * dist;

	  ACE_Time_Value timeDelta = timeStampR_ - prevTimeStampR_;

	  double deltaT = (double)timeDelta.sec() + (double)timeDelta.usec() / 1000000.;

	  // compose odometry message
	  Miro::timeA2C(timeStampR_, status_.time);

	  if (!Sparrow::Parameters::instance()->goalie) {
	    status_.position.point.x = (long) xPos_;
	    status_.position.point.y = (long) yPos_;
	  }
	  else {
	    if (Sparrow::Parameters::instance()->sparrow2003){
               status_.position.point.x = (long) -yPos_;
	       status_.position.point.y = (long) xPos_;
	    }
	    else{
	       status_.position.point.x = (long) yPos_;
	       status_.position.point.y = (long) -xPos_;
	    }
	  }
	  status_.velocity.translation = (long)(dist / deltaT);
	  status_.velocity.rotation = turn / deltaT;

	  //cout << status_.time.sec<< "  " << status_.time.usec <<endl; //(dL+dR) / 2*dtime<<  endl;

// berechnung
if (false) {
	  ACE_Time_Value dTR = timeStampR_ - prevTimeStampR_;
	  deltaTR[counter] = (double)dTR.sec() + (double)dTR.usec() / 1000000.;

	  ACE_Time_Value dTL = timeStampL_ - prevTimeStampL_;
	  deltaTL[counter] = (double)dTL.sec() + (double)dTL.usec()/ 1000000.;

	  ACE_Time_Value dTLR = (timeStampL_ > timeStampR_)?
		                timeStampL_ - timeStampR_ :
				timeStampR_ - timeStampL_;
	  deltaTLR[counter] = (double)dTLR.sec() + (double)dTLR.usec()/ 1000000.;

	  cout << "TimerOdo dTR: " << dTR << " \tdTL: " << dTL << " \tdTLR: " << dTLR << endl;

	  int i;
	  if (counter ==  0) {
	    double meanL = deltaTL[0];
	    double meanR = deltaTR[0];
	    double meanLR = deltaTLR[0];

	    for ( i = 49; i > 0; --i) {
	      meanL += deltaTL[i];
	      meanR += deltaTR[i];
	      meanLR += deltaTLR[i];
	    }

	    meanL /= 50.;
	    meanR /= 50.;
	    meanLR /= 50.;

	    double varL = 0.;
	    double varR = 0.;
	    double varLR = 0.;

	    for ( i = 49; i >= 0; --i) {
	      varL += (deltaTL[i] - meanL) * (deltaTL[i] - meanL);
	      varR += (deltaTR[i] - meanR) * (deltaTR[i] - meanR);
	      varLR += (deltaTLR[i] - meanLR) * (deltaTLR[i] - meanLR);
	    }

	    varL /= 49.;
	    varR /= 49.;
	    varLR /= 49.;

	    cout << "TimerOdo L:  mean=" << meanL << "sec \t var=" <<sqrt(varL)<< endl;
	    cout << "TimerOdo R:  mean=" << meanR << "sec \t var=" <<sqrt(varR)<< endl;
	    cout << "TimerOdo LR: mean=" << meanLR << "sec \t var=" <<sqrt(varLR)<< endl;

	    counter = 50;
	  }


	  --counter;
}
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
}
