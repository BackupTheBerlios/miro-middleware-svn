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
#include "RangeSensorAvoid.h"

#include "idl/RangeSensorC.h"
#include "idl/RangeEventC.h"
#include "miro/Client.h"
#include "miro/Angle.h"

#include "botsvcs/BAP/MotionArbiterMessage.h"

#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;

using Miro::RangeSensor;
using Miro::RangeSensor_ptr;
using Miro::ScanDescriptionIDL_var;
using Miro::BAP::MotionArbiterMessage;
using Miro::deg2Rad;

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(RangeSensorAvoid, RangeSensorAvoidParameters)

RangeSensorAvoid::RangeSensorAvoid(Miro::Client& _client,
				   EventChannel_ptr _ec,
				   const string& _name,
				   const string& _domainName,
				   Miro::StructuredPushSupplier * _pSupplier) :
  Super(_client, _ec, _name, _domainName, _pSupplier)
{
  cout << "Constructing " << _name << endl;
}

void
RangeSensorAvoid::action()
{
  Miro::Guard guard(mutex_);

  Super::action();

  const RangeSensorAvoidParameters * params =
    dynamic_cast<const RangeSensorAvoidParameters *>(params_);


  //  cout << "RangeSensorAvoid: evaluating data" << endl;

  // get the minimum distance measurement

  double minLeft = evalScanMin(deg2Rad(45), deg2Rad(135));
  double minLeftFront = evalScanMin(deg2Rad(30), deg2Rad(60));
  double minFront = evalScanMin(deg2Rad(-45), deg2Rad(45));
  double minRightFront = evalScanMin(deg2Rad(-60), deg2Rad(-30));
  double minRight = evalScanMin(deg2Rad(-135), deg2Rad(-45));
 
  if (minLeft < 0.)
    minLeft = params->lateralDistance * 10.;
  if (minLeftFront < 0.)
    minLeftFront = params->minDistance * 10.;
  if (minFront < 0.)
    minRightFront = params->minDistance * 10.;
  if (minRightFront < 0.)
    minRightFront = params->minDistance * 10.;
  if (minRight < 0.)
    minRight = params->lateralDistance * 10.;
 
  cout << "Avoid scan: " 
       << minLeft << " \t" 
       << minLeftFront << " \t"
       << minFront << " \t"
       << minRightFront << " \t"
       << minRight << endl;

  // build default message;
  MotionArbiterMessage message;
  message.id = this;
  message.active = false;
  message.velocity.translation = params->translation;
  message.velocity.rotation = 0;

  //  cout << "RangeSensorAvoid: selecting action" << endl;

  // front blocked
  if (minFront < params->minDistance) {
    message.active = true;

    if (minFront < params->haltDistance ||
	minLeftFront < params->lateralDistance ||
	minRightFront < params->lateralDistance)
      message.velocity.translation = 0;

    message.velocity.rotation = (std::min(minLeft, minLeftFront) >
				 std::min(minRight, minRightFront))?
      params->rotation :
      - params->rotation;
  }
  // left blocked
  else if (minLeftFront < params->lateralDistance &&
	   !(minRightFront < params->lateralDistance)) {
    message.active = true;
    message.velocity.rotation = -params->rotation;
  }
  // right blocked
  else if (minRightFront < params->lateralDistance &&
	   !(minLeftFront < params->lateralDistance)) {
    message.active = true;
    message.velocity.rotation = params->rotation;
  }

  //  cout << "RangSensorAvoid: action." << endl;	   
  arbitrate(message);
}

double
RangeSensorAvoid::evalScanMin(double alpha, double beta)
{
  EgoMap::const_iterator first = egoMap_.lower_bound(alpha);
  EgoMap::const_iterator last = egoMap_.upper_bound(beta);

  double minimum = Miro::RangeSensor::HORIZON_RANGE;
  if (first != last) {
    minimum = abs(first->second);
    for (++first; first != last; ++first) {
      double dist = abs(first->second);
      if (dist < minimum)
	minimum = dist;
    }
  }
  return minimum;
}



