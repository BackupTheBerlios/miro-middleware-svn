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
//#include "miro/RangeSensorC.h"

#include "TurnRight.h"

#include <algorithm>

using std::cout;
using std::string;

using Miro::RangeSensor;
using Miro::RangeSensor_ptr;
using Miro::RangeGroupEventIDL;
using Miro::MotionArbiterMessage;

namespace 
{
  const string behaviourName("TurnRight");
};


TurnRight::TurnRight(ACE_Reactor& _reactor, RangeSensor_ptr _laser) :
  Super(_reactor),
  laser_(RangeSensor::_duplicate(_laser))
{
  cout << "Constructing TurnRight behaviour." << endl;
}

TurnRightParameters *
TurnRight::getParametersInstance()
{
  return new TurnRightParameters();
}

void
TurnRight::action()
{
  const TurnRightParameters * params =
    dynamic_cast<const TurnRightParameters *>(params_);
  RangeGroupEventIDL * scan = laser_->getGroup(0);

  // search maximum right-frontal laser value, to decide wether to turn or not
  //int i = (params->leftLaser+params->rightLaser)/2; //check only 90 deg
  int i = params->leftLaser;
  int j = (params->leftLaser + params->rightLaser)/2; //check only 90 deg
  //  int j = params->rightLaser;
  if (i > j)
    std::swap(i, j);

  int maximum = scan->range[i];
  int id = i;
  for (++i; i <= j; ++i) {
    if ((scan->range[i] - maximum)>5) { 
      maximum = scan->range[i];
      id = i;
    }
  }

  MotionArbiterMessage message;

  message.id = this;

  int LASER_NUM=laser_->getScanDescription()->group[0].sensor.length();

  if (abs(id - (LASER_NUM/2))>5) { //keep a frontal 5 degree margin
    message.active=true;
    message.velocity.translation = params->translation; 
    message.velocity.rotation = (id<(LASER_NUM/2))?params->rotation:-params->rotation;
  } else {
    message.active=true;
    message.velocity.translation = params->translation;
    message.velocity.rotation = 0;
  }

  arbitrate(message);
}

const string&
TurnRight::getBehaviourName() const
{
  return behaviourName;
}

