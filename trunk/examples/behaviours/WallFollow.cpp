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
		       const string& _domainName,
		       Miro::StructuredPushSupplier * _pSupplier) :
  Super(_client, _ec, _name, _domainName, _pSupplier)
{
  cout << "Constructing " << _name << endl;
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
  rL = regressionsGerade(left_, Miro::deg2Rad(90), mL, bL);
  //    rL = regressionsGerade(leftFront_, Miro::deg2Rad(45), mL, bL);

  rR = regressionsGerade(right_, Miro::deg2Rad(-90), mR, bR);
  //    rR = regressionsGerade(rightFront_, Miro::deg2Rad(-45), mR, bR);

  //  cout << name_ << ": selecting action" << endl;
 
  // follow left wall
  if ( rL && 
       ( !rR ||
	 bL < bR ) ) {

    double alpha = atan2(mL, 1.);
    double dist = cos(alpha) * bL;

    cout << name_ << " left: alpha=" << Miro::rad2Deg(alpha) 
	 << "�\t d=" << dist << "mm" << endl;

    message.active = true;        // yes, we do something
    
    if (dist < params->minDistance) { // we are too near
      alpha -= M_PI / 6.; // -30�
      cout << name_ << " left: too near alpha" << Miro::rad2Deg(alpha) << "�" << endl;
    }
    else if (dist > params->maxDistance) { // we are too far
      alpha += M_PI / 6.; // +30�
      cout << name_ << " left: too far alpha" << Miro::rad2Deg(alpha) << "�" << endl;
    }

    message.velocity.rotation = min(params->rotation, 
				    max(-params->rotation, alpha));
  }
  else if (rR) {
    double alpha = atan2(mR, 1.);
    double dist = cos(alpha) * bR;

    cout << name_ << " right: alpha=" << Miro::rad2Deg(alpha)
	 << "�\t d=" << dist << "mm" << endl;

    message.active = true;

    if (dist < params->minDistance) { // we are too near
      alpha += M_PI / 6.; // +30�
      cout << name_ << " right: too near alpha" << Miro::rad2Deg(alpha) << "�" << endl;
    }
    else if (dist > params->maxDistance) { // we are too far
      alpha -= M_PI / 6.; // -30�
      cout << name_ << " right: too far alpha" << Miro::rad2Deg(alpha) << "�"  << endl;
    }

    message.velocity.rotation = min(params->rotation, 
				    max(-params->rotation, alpha));
  }

  //  cout << name_ << ": action." << endl;
  arbitrate(message);
}

bool
WallFollow::regressionsGerade(const SensorScan& _scan, double delta,
			      double& a, double& b) const
{
  if (_scan.size() < 5) // too litle data
    return false;

  // correction angle
  delta += heading_ - M_PI / 2.;
  Vector2d alpha(cos(-delta), sin(-delta));

  // build vector of egocentric sensor readings
  SensorScan scan(_scan);
  SensorScan::iterator first, last = scan.end();
  for (first = scan.begin(); first != last; ++first) {
    (*first) -= position_;
    (*first) *= alpha;

    //    cout << "x =" << first->real() << "\t y=" << first->imag() << endl;
  }

  // Geradengleichung y = m*x + b
  Vector2d sum = std::accumulate(scan.begin(), scan.end(), Vector2d());

  //  cout << "sum: " << sum.real() << ", " << sum.imag() << endl;

  double n = scan.size();
  double xmean = sum.real() / n;
  double ymean = sum.imag() / n;
  double sxy = 0.;
  double ssqx = 0.;

  for (first = scan.begin(); first != last; ++first) {
    ssqx += first->real() * first->real();
    sxy += first->real() * first->imag();
  }

  double d = ssqx - n * xmean * xmean;

  //  cout << "x*x Sum=" << ssqx << " \tpSum=" << sxy << "\td=" << d << endl;

  if (fabs(d) < 0.0001)
    return false;

  a = (sxy - n * xmean * ymean) / d;
  b = ymean - a * xmean;

  return true;
}