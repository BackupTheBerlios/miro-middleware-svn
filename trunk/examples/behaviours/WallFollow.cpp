// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
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
using std::max;
using std::min;

using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;

using Miro::RangeSensor;
using Miro::RangeSensor_ptr;
using Miro::ScanDescriptionIDL_var;
using Miro::MotionArbiterMessage;

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(WallFollow, WallFollowParameters)

WallFollow::WallFollow(Miro::Client& _client,
		       EventChannel_ptr _ec,
		       const string& _name,
		       const string& _domainName,
		       Miro::StructuredPushSupplier * _pSupplier) :
  Super(_client, _ec, _name, _domainName, NULL)
{
  cout << "Constructing " << _name << endl;
}

void
WallFollow::action()
{
  Miro::Guard guard(mutex_);

  Super::action();

  const WallFollowParameters * params =
    dynamic_cast<const WallFollowParameters *>(params_); 

  //  cout << name_ << ": evaluating data" << endl;

  // build default message;
  MotionArbiterMessage message;
  message.id = this;
  message.active = false;
  message.velocity.translation = params->translation;
  message.velocity.rotation = 0.;

  bool rL, rR;
  double mL = 0.;
  double bL = 0.;
  double mR = 0.;
  double bR = 0.;

  //  cout << name_ << ": Regressionsgeraden" << endl;
  // left wall 
  rL = regressionsGerade(Miro::deg2Rad(30), Miro::deg2Rad(150), Miro::deg2Rad(90), mL, bL);
  //    rL = regressionsGerade(leftFront_, Miro::deg2Rad(45), mL, bL);

  rR = regressionsGerade(Miro::deg2Rad(-150), Miro::deg2Rad(-30), Miro::deg2Rad(-90), mR, bR);
  //    rR = regressionsGerade(rightFront_, Miro::deg2Rad(-45), mR, bR);

  //  cout << name_ << ": selecting action" << endl;
 
  // follow left wall
  if ( rL && 
       ( !rR ||
	 bL < bR ) ) {

    double alpha = atan2(mL, 1.);
    double dist = cos(alpha) * bL;

    cout << name_ << " left: alpha=" << Miro::rad2Deg(alpha) 
	 << "°\t d=" << dist << "mm" << endl;

    message.active = true;        // yes, we do something
    
    if (dist < params->minDistance) { // we are too near
      alpha -= M_PI / 6.; // -30°
      cout << name_ << " left: too near alpha" << Miro::rad2Deg(alpha) << "°" << endl;
    }
    else if (dist > params->maxDistance) { // we are too far
      alpha += M_PI / 6.; // +30°
      cout << name_ << " left: too far alpha" << Miro::rad2Deg(alpha) << "°" << endl;
    }

    message.velocity.rotation = min(params->rotation, 
				    max(-params->rotation, alpha));
  }
  else if (rR) {
    double alpha = atan2(mR, 1.);
    double dist = cos(alpha) * bR;

    cout << name_ << " right: alpha=" << Miro::rad2Deg(alpha)
	 << "°\t d=" << dist << "mm" << endl;

    message.active = true;

    if (dist < params->minDistance) { // we are too near
      alpha += M_PI / 6.; // +30°
      cout << name_ << " right: too near alpha" << Miro::rad2Deg(alpha) << "°" << endl;
    }
    else if (dist > params->maxDistance) { // we are too far
      alpha -= M_PI / 6.; // -30°
      cout << name_ << " right: too far alpha" << Miro::rad2Deg(alpha) << "°"  << endl;
    }

    message.velocity.rotation = min(params->rotation, 
				    max(-params->rotation, alpha));
  }

  arbitrate(message);
}

bool
WallFollow::regressionsGerade(double _alpha, double _beta, double delta,
			      double& a, double& b) const
{
  // correction angle
  Vector2d alpha(cos(-delta), sin(-delta));

  // build vector of egocentric sensor readings
  std::vector<Vector2d> scan;
  scan.reserve(scan_.size());

  EgoMap::const_iterator first, last = egoMap_.upper_bound(_beta);
  for (first = egoMap_.lower_bound(_alpha); first != last; ++first) {

    scan.push_back(first->second * alpha);

    //    cout << "x =" << first->real() << "\t y=" << first->imag() << endl;
  }

  if (scan.size() < 5) { // too litle data
    cout << "too little data" << endl;
    return false;
  }

  // Geradengleichung y = m*x + b
  Vector2d sum = std::accumulate(scan.begin(), scan.end(), Vector2d());

  //  cout << "sum: " << sum.real() << ", " << sum.imag() << endl;

  double n = scan.size();
  double xmean = sum.real() / n;
  double ymean = sum.imag() / n;
  double sxy = 0.;
  double ssqx = 0.;

  std::vector<Vector2d>::const_iterator i, iEnd = scan.end();
  for (i = scan.begin(); i != iEnd; ++i) {
    ssqx += i->real() * i->real();
    sxy += i->real() * i->imag();
  }

  double d = ssqx - n * xmean * xmean;

  //  cout << "x*x Sum=" << ssqx << " \tpSum=" << sxy << "\td=" << d << endl;

  if (fabs(d) < 0.0001)
    return false;

  a = (sxy - n * xmean * ymean) / d;
  b = ymean - a * xmean;

  return true;
}
