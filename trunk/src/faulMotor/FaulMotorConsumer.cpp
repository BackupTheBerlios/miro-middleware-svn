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
#include <fstream>

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

  std::ofstream ticksFile;

  bool const FAUL_LOGGING = false;

  double const Consumer::CLOCK_2_SEC = 0.00933;

  //------------------------//
  //----- constructors -----//
  //------------------------//
  Consumer::Consumer(Connection * _pConnection,
		     Miro::OdometryImpl * _pOdometry):
    params_(Parameters::instance()),
    pConnection_(_pConnection),
    pOdometry_(_pOdometry),
    init_(2),
    xPos_(0.),
    yPos_(0.),
    ticksL_(0.),
    ticksR_(0.),
    prevTicksL_(0.),
    prevTicksR_(0.),
    wheelBase_(params_->motion.wheelBase),
    oddWheel_(0)
  {
    DBG(cout << "Constructing FaulMotorConsumer." << endl);
    status_.position.point.x = 0.;
    status_.position.point.y = 0.;
    status_.position.heading = 0.;
    status_.velocity.translation = 0;
    status_.velocity.rotation = 0.;

    if (FAUL_LOGGING)
      ticksFile.open("ticks.log");
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//
  Consumer::~Consumer()
  {
    DBG(cout << "Destructing FaulMotorConsumer." << endl);
    if (FAUL_LOGGING)
      ticksFile.close();
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

      switch (pFaulMsg->wheel()) {
      case FaulController::OdometryMessage::LEFT:
	prevTicksL_ = ticksL_;
	ticksL_ = pFaulMsg->ticks();
	deltaTicksL_ = ticksL_ - prevTicksL_;
	clockL_ = pFaulMsg->clock();
	prevTimeStampL_ = timeStampL_;
	timeStampL_ = pFaulMsg->time();
	break;
	
      case FaulController::OdometryMessage::RIGHT:
	prevTicksR_ = ticksR_;
	ticksR_ = pFaulMsg->ticks();
	deltaTicksR_ = ticksR_ - prevTicksR_;
	clockR_ = pFaulMsg->clock();
	prevTimeStampR_ = timeStampR_;
	timeStampR_ = pFaulMsg->time();
	break;
	
      case FaulController::OdometryMessage::DELTA_LEFT:
	deltaTicksL_ = pFaulMsg->ticks();
	clockL_ = pFaulMsg->clock();
	prevTimeStampL_ = timeStampL_;
	timeStampL_ = pFaulMsg->time();
	break;

      case FaulController::OdometryMessage::DELTA_RIGHT:
	deltaTicksR_ = pFaulMsg->ticks();
	clockR_ = pFaulMsg->clock();
	prevTimeStampR_ = timeStampR_;
	timeStampR_ = pFaulMsg->time();
	break;
      }

      ++oddWheel_; 
      oddWheel_ &= 1;

      if ((params_->odometryPolling && !oddWheel_) || // new odometry mode

	  (prevTimeStampL_+ ACE_Time_Value(0, 10000) < timeStampR_ && // old odo
	   prevTimeStampR_+ ACE_Time_Value(0, 10000) < timeStampL_ &&
	   prevTimeStampL_ != timeStampL_ &&
	   prevTimeStampR_ != timeStampR_)) {

	// if polling mode send next request 
	// along with the current acc/vel set points
	if (params_->odometryPolling)
	  pConnection_->deferredSetSpeed();

	if (init_ == 0) {
	  if (params_->odometryPolling)
	    integrateBinary();
	  else
	    integrateAscii();

	  // statistical evaluation of odometry timing
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

	  pOdometry_->integrateData(status_);
	  return;
	}
	else {
	  --init_;
	}
      }
    }
  }

  void
  Consumer::integrateBinary() 
  {
    // sanity checking for clock ticks
    // 
    // the faulhaber controller can only count to 9, regarding clock ticks
    // so we have to make sure, we didn't miss 1/10 of a second...
    
    if (FAUL_LOGGING && clockL_ == 0)
      ticksFile << "left  ticks == 0" << std::endl;
    if (FAUL_LOGGING && clockR_ == 0)
      ticksFile << "right ticks == 0" << std::endl;
    
    bool overflow = false;
    ACE_Time_Value jitter(0, 50000); // 1/20 sec
    ACE_Time_Value deltaTimeL = timeStampL_ - prevTimeStampL_;
    
    if (abs(clockL_ - clockR_) > 4) {
      ACE_Time_Value t;
      t.set((double)clockL_ * CLOCK_2_SEC);
      while (deltaTimeL > t + jitter) {
	overflow = true;
	clockL_ += 9;
	t.set((double)clockL_ * CLOCK_2_SEC);
      }
      ACE_Time_Value deltaTimeR = timeStampR_ - prevTimeStampR_;
      t.set((double)clockR_ * CLOCK_2_SEC);
      while (deltaTimeR > t + jitter) {
	overflow = true;
	clockR_ += 9;
	t.set((double)clockR_ * CLOCK_2_SEC);
      }
    }
        
    // lowlevel logging hook
    if (FAUL_LOGGING) {
      ticksFile << timeStampL_ << " " 
		<< (timeStampL_ - prevTimeStampL_) << " " 
		<< clockL_ << " " 
		<< ticksL_ << " " << deltaTicksL_ << "\t"
		<< timeStampR_ << " " 
		<< (timeStampR_ - prevTimeStampR_) << " " 
		<< clockR_ << " " 
		<< ticksR_ << " " << deltaTicksR_;
      
      if (overflow)
	ticksFile << "   !!!!!!!!!!!";
      
      if (abs(clockL_ - clockR_) > 5)
	ticksFile << "   ???????????";
      
      ticksFile << std::endl;
    }

    
    // okay, lets do business
    double dL = -(deltaTicksL_) / params_->leftTicksPerMM;
    double dR = (deltaTicksR_) / params_->rightTicksPerMM;
    
    // correct clock jitter from both wheels
    double mean = ((double)(clockL_ + clockR_)) / 2.;
    double deltaT = mean * CLOCK_2_SEC;
    
    if (clockL_ != clockR_) {
      
      dL *= mean/((double)clockL_);
      dR *= mean/((double)clockR_);
    }

    odometryUpdate(dL, dR, deltaT);
  }

  void
  Consumer::integrateAscii() 
  {
    double dL = -(deltaTicksL_) / params_->leftTicksPerMM;
    double dR = (deltaTicksR_) / params_->rightTicksPerMM;
    ACE_Time_Value timeDelta = timeStampR_ - prevTimeStampR_;
    double deltaT = (double)timeDelta.sec() + (double)timeDelta.usec() / 1000000.;

    odometryUpdate(dL, dR, deltaT);
  }

  void
  Consumer::odometryUpdate(double _dL, double _dR, double _deltaT)
  {
    // calculate new orientation
    double turn = (_dR - _dL) / wheelBase_;
    status_.position.heading += turn;
    
    Miro::Angle::normalize(status_.position.heading);
    
    double dist = (_dL + _dR) / 2.;
    xPos_ += cos(status_.position.heading) * dist;
    yPos_ += sin(status_.position.heading) * dist;
    
    
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
    status_.velocity.translation = (long)(dist / _deltaT);
    status_.velocity.rotation = turn / _deltaT;
    
    //cout << status_.time.sec<< "  " << status_.time.usec <<endl; //(dL+dR) / 2*dtime<<  endl;
  }
}
