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

  bool const FAUL_LOGGING = true;

  double const Consumer::CLOCK_2_SEC = 0.00933;

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

      if (pFaulMsg->wheel() == FaulController::OdometryMessage::LEFT) {
	ticksL_ = pFaulMsg->ticks();
	clockL_ = pFaulMsg->clock();
	timeStampL_ = pFaulMsg->time();
      }
      else {
	ticksR_ = pFaulMsg->ticks();
	clockR_ = pFaulMsg->clock();
	timeStampR_ = pFaulMsg->time();
      }

      ++oddWheel_; 
      oddWheel_ &= 1;

      if (!oddWheel_) {
	if (init_ == 0) {
	  double deltaTicksL = ticksL_ - prevTicksL_;
	  double deltaTicksR = ticksR_ - prevTicksR_;

	  // sanity checking for clock ticks
	  // 
	  // the faulhaber controller can only count to 9, regarding clock ticks
	  // so we have to make sure, we didn't miss 1/10 of a second...

	  if (FAUL_LOGGING && clockL_ == 0)
	    ticksFile << "left  ticks == 0=" << std::endl;
	  if (FAUL_LOGGING && clockR_ == 0)
	    ticksFile << "right ticks == 0=" << std::endl;

	  bool overflow = false;
	  ACE_Time_Value jitter(0, 50000); // 1/20 sec
	  ACE_Time_Value deltaTimeL = timeStampL_ - prevTimeStampL_;

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


	  // lowlevel logging hook
	  if (FAUL_LOGGING) {
	    ticksFile << timeStampL_ << " " 
		      << (timeStampL_ - prevTimeStampL_) << " " 
		      << clockL_ << " " 
		      << ticksL_ << " " << deltaTicksL << "\t"
		      << timeStampR_ << " " 
		      << (timeStampR_ - prevTimeStampR_) << " " 
		      << clockR_ << " " 
		      << ticksR_ << " " << deltaTicksR;

	    if (overflow)
	      ticksFile << "   !!!!!!!!!!!";

	    if (abs(clockL_ - clockR_) > 5)
	      ticksFile << "   ???????????";

	    ticksFile << std::endl;
	  }


	  // okay, lets do business
	  double dL = -(ticksL_ - prevTicksL_) / params_->leftTicksPerMM;
	  double dR = (ticksR_ - prevTicksR_) / params_->rightTicksPerMM;

	  // correct clock jitter from both wheels
	  double mean = ((double)(clockL_ + clockR_)) / 2.;
	  deltaT_ = mean * CLOCK_2_SEC;

	  if (clockL_ != clockR_) {

	    dL *= mean/((double)clockL_);
	    dR *= mean/((double)clockR_);
	  }

	  // calculate new orientation
	  double turn = (dR - dL) / wheelBase_;
	  status_.position.heading += turn;

	  Miro::Angle::normalize(status_.position.heading);

	  double dist = (dL + dR) / 2.;
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
	  status_.velocity.translation = (long)(dist / deltaT_);
	  status_.velocity.rotation = turn / deltaT_;

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
