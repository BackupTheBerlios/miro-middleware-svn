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
#include "WallFollow.h"

#include "miro/RangeSensorC.h"
#include "miro/RangeEventC.h"
#include "miro/Client.h"
#include "miro/MotionArbiterMessage.h"
#include "miro/Angle.h"

#include <numeric>

using std::cout;
using std::cerr;
using std::string;

using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;

using Miro::RangeSensor;
using Miro::RangeSensor_ptr;
using Miro::ScanDescriptionIDL_var;
using Miro::MotionArbiterMessage;

WallFollow::WallFollow(Miro::Client& _client,
				   EventChannel_ptr _ec,
				   const string& _name,
				   const string& _domainName) :
  Super(_client, _ec, _name, _domainName)
{
}

WallFollowParameters *
WallFollow::getParametersInstance()
{
  return new WallFollowParameters();
}

void
WallFollow::action()
{
  Miro::Guard guard(mutex_);

  Super::action();

  const WallFollowParameters * params =
    dynamic_cast<const WallFollowParameters *>(params_); 

  cout << name_ << ": evaluating data" << endl;

  // build default message;
  MotionArbiterMessage message;
  message.id = this;
  message.active = false;
  message.velocity.translation = params->translation;
  message.velocity.rotation = 0;

  bool rL, rR;
  double mL = 0.;
  double bL = 0.;
  double mR = 0.;
  double bR = 0.;

  cout << name_ << ": Regressionsgeraden" << endl;

  // left wall 
  if (!(rL = regressionsGerade(left_, Miro::deg2Rad(90), mL, bL)))
    rL = regressionsGerade(leftFront_, Miro::deg2Rad(45), mL, bL);

  if (!(rR = regressionsGerade(right_, Miro::deg2Rad(-90), mR, bR)))
    rR = regressionsGerade(rightFront_, Miro::deg2Rad(-45), mR, bR);


  cout << name_ << ": selecting action" << endl;
 
  // follow left wall
  if ( rL && 
       ( !rR ||
	 bL < bR ) ) {

    double alpha = atan2(mL, 1.);

    cout << name_ << " left: alpha=" << Miro::rad2Deg(alpha) 
	 << "°\t b=" << bL << "mm" << endl;

    message.active = true;        // yes, we do something
    
    if (bL < params->minDistance) // we are too near
      alpha -= M_PI / 4.; // -45°
    else if (bL > params->maxDistance) // we are too far
      alpha += M_PI / 4.; // +45°

    message.velocity.rotation = min(params->rotation, mL);
  }
  else if (rR) {
    double alpha = atan2(mR, 1.);

    cout << name_ << " right: alpha=" << Miro::rad2Deg(alpha)
	 << "°\t b=" << bR << "mm" << endl;

    message.active = true;

    if (bL < params->minDistance) // we are too near
      mR += M_PI / 4.; // +45°
    else if (bL > params->maxDistance) // we are too far
      mR -= M_PI / 4.; // -45°

    message.velocity.rotation = max(-params->rotation, mR);
  }

  cout << name_ << ": action." << endl;
  arbitrate(message);
}

bool
WallFollow::regressionsGerade(const SensorScan& _scan, double delta,
			      double& m, double& b) const
{
  if (_scan.size() < 5) // too litle data
    return false;

  // correction angle
  delta += heading_;
  Vector2d alpha(cos(-delta), sin(-delta));

  // build vector of egocentric sensor readings
  cout << name_ << ": local scan copy" << _scan.size() << endl;
  SensorScan scan(_scan);
  cout << name_ << ": egocentric mapping" << endl;
  SensorScan::iterator first, last = scan.end();
  for (first = scan.begin(); first != last; ++first) {
    (*first) -= position_;
    (*first) *= alpha;
  }

  cout << name_ << ": accumulate" << endl;
  // Geradengleichung y = m*x + b
  Vector2d sum = std::accumulate(scan.begin(), scan.end(), Vector2d());

  double n = scan.size();
  double productSum = 0.;
  double xSquareSum = 0.;

  cout << name_ << ": summing up" << endl;
  for (first = scan.begin(); first != last; ++first) {
    xSquareSum += first->real() * first->real();
    productSum += first->real() * first->imag();
  }

  double d = n * xSquareSum - sum.real() * sum.real();

  if (fabs(d) < 0.0001)
    return false;

  m = ( n * productSum - sum.real() * sum.imag() ) / d;
  b = sum.imag() - m * sum.real();
  b /= n;

  return true;
}
