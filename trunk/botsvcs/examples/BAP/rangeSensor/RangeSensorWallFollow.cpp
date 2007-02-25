// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "RangeSensorWallFollow.h"

#include "idl/RangeSensorC.h"
#include "idl/RangeEventC.h"
#include "miro/Client.h"
#include "miro/Angle.h"

#include "botsvcs/BAP/MotionArbiterMessage.h"

#include <numeric>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::max;
using std::min;

using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;

using Miro::RangeSensor;
using Miro::RangeSensor_ptr;
using Miro::ScanDescriptionIDL_var;
using Miro::BAP::MotionArbiterMessage;

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(RangeSensorWallFollow, RangeSensorWallFollowParameters)

RangeSensorWallFollow::RangeSensorWallFollow(Miro::Client& _client,
		       EventChannel_ptr _ec,
		       const string& _name,
		       const string& _domainName,
		       Miro::StructuredPushSupplier * ) :
  Super(_client, _ec, _name, _domainName, NULL)
{
  cout << "Constructing " << _name << endl;
}

void
RangeSensorWallFollow::action()
{
  Miro::Guard guard(mutex_);

  Super::action();

  const RangeSensorWallFollowParameters * params =
    dynamic_cast<const RangeSensorWallFollowParameters *>(params_); 

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
	 << ",A0(B\t d=" << dist << "mm" << endl;

    message.active = true;        // yes, we do something
    
    if (dist < params->minDistance) { // we are too near
      alpha -= M_PI / 6.; // -30,A0(B
      cout << name_ << " left: too near alpha" << Miro::rad2Deg(alpha) << ",A0(B" << endl;
    }
    else if (dist > params->maxDistance) { // we are too far
      alpha += M_PI / 6.; // +30,A0(B
      cout << name_ << " left: too far alpha" << Miro::rad2Deg(alpha) << ",A0(B" << endl;
    }

    message.velocity.rotation = min(params->rotation, 
				    max(-params->rotation, alpha));
  }
  else if (rR) {
    double alpha = atan2(mR, 1.);
    double dist = cos(alpha) * bR;

    cout << name_ << " right: alpha=" << Miro::rad2Deg(alpha)
	 << ",A0(B\t d=" << dist << "mm" << endl;

    message.active = true;

    if (dist < params->minDistance) { // we are too near
      alpha += M_PI / 6.; // +30,A0(B
      cout << name_ << " right: too near alpha" << Miro::rad2Deg(alpha) << ",A0(B" << endl;
    }
    else if (dist > params->maxDistance) { // we are too far
      alpha -= M_PI / 6.; // -30,A0(B
      cout << name_ << " right: too far alpha" << Miro::rad2Deg(alpha) << ",A0(B"  << endl;
    }

    message.velocity.rotation = min(params->rotation, 
				    max(-params->rotation, alpha));
  }

  arbitrate(message);
}

bool
RangeSensorWallFollow::regressionsGerade(double _alpha, double _beta, double delta,
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
