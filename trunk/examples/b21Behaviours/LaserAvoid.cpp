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

#include "miro/ActionPattern.h"
#include "miro/Arbiter.h"
#include "miro/MotionArbiterMessage.h"

#include "LaserAvoid.h"

#include <algorithm>

using std::cout;
using std::string;

using Miro::RangeSensor;
using Miro::RangeSensor_ptr;
using Miro::RangeGroupEventIDL;
using Miro::MotionArbiterMessage;

const string LaserAvoid::name_ = "LaserAvoid";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(LaserAvoid, LaserAvoidParameters)

LaserAvoid::LaserAvoid(ACE_Reactor& _reactor, RangeSensor_ptr _laser) :
  Super(_reactor),
  laser_(RangeSensor::_duplicate(_laser))
{
  cout << "Constructing LaserAvoid behaviour." << endl;
}

void
LaserAvoid::action()
{
  const LaserAvoidParameters * params =
    dynamic_cast<const LaserAvoidParameters *>(params_);
  RangeGroupEventIDL * scan = laser_->getGroup(0);

  // search minimum frontal laser value, to decide wether to avoid or not
  int i = params->leftLaser;
  int j = params->rightLaser;
  if (i > j)
    std::swap(i, j);

  int minimum = scan->range[i];
  int id = i;
  for (++i; i <= j; ++i) {
    if (scan->range[i] < minimum) {
      minimum = scan->range[i];
      id = i;
    }
  }
  //    int minimum = std::min_element(scan.begin() + i, scan.begin() + j + 1);
  //   cout << "minimum: " << minimum << endl;

  MotionArbiterMessage message;

  message.id = this;
  if (minimum < params->minDistance) {
    message.active = true;
    message.velocity.translation = 
      (minimum < params->haltDistance)? 0 : params->translation;
    message.velocity.rotation = 
      (abs(static_cast<int>(params->leftLaser - id)) >
      abs(static_cast<int>(params->rightLaser - id)))?
	params->rotation : -params->rotation;
  }
  else
    message.active = false;
  
  arbitrate(message);
}

const string&
LaserAvoid::getBehaviourName() const
{
  return name_;
}
