// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "botsvcs/BAP/ActionPattern.h"
#include "botsvcs/BAP/Arbiter.h"
#include "botsvcs/BAP/MotionArbiterMessage.h"
#include "miro/Log.h"

#include "LaserAvoid.h"

#include <string>

const std::string LaserAvoid::name_ = "LaserAvoid";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(LaserAvoid, LaserAvoidParameters)

LaserAvoid::LaserAvoid(ACE_Reactor& _reactor, 
		       Miro::RangeSensor_ptr _laser) :
  Super(_reactor),
  laser_(Miro::RangeSensor::_duplicate(_laser))
{
}

void
LaserAvoid::action()
{
  // get parameters
  const LaserAvoidParameters * params =
    dynamic_cast<const LaserAvoidParameters *>(params_);
  // make sure nothing went wrong
  MIRO_ASSERT(params != NULL);

  // get laser scan
  Miro::RangeGroupEventIDL * scan = laser_->getGroup(0);

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

  // create arbiter message
  Miro::BAP::MotionArbiterMessage message;

  // init arbiter message
  message.id = this;
  // get active if there is an obstacle
  if (minimum < params->minDistance) {
    message.active = true;
    // stop if we are too close to the obstacle
    message.velocity.translation = 
      (minimum < params->haltDistance)? 0 : params->translation;
  // turn away from obstacle
    message.velocity.rotation = 
      (abs(static_cast<int>(params->leftLaser - id)) >
      abs(static_cast<int>(params->rightLaser - id)))?
	params->rotation : -params->rotation;
  }
  else
    message.active = false;
  
  arbitrate(message);
}

const std::string&
LaserAvoid::getBehaviourName() const
{
  return name_;
}
