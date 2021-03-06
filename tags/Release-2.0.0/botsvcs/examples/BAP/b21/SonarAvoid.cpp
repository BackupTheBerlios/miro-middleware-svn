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
#include "SonarAvoid.h"

#include "idl/RangeEventC.h"
#include "botsvcs/BAP/MotionArbiterMessage.h"

const std::string SonarAvoid::name_ = "SonarAvoid";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(SonarAvoid, SonarAvoidParameters)

SonarAvoid::SonarAvoid(Miro::RangeSensor_ptr _rangeSensor,
		       CosNotifyChannelAdmin::EventChannel_ptr _ec,
		       const std::string& _domainName) :
  Super(_ec),
  sensor_()
{
  std::cout << "Constructing SonarAvoid behaviour." << std::endl;

  // set up the sensor scan buffer
  Miro::ScanDescriptionIDL_var scanDescription = 
    _rangeSensor->getScanDescription();
  if (scanDescription->group.length() > 0)
    sensor_.resize(scanDescription->group[0].sensor.length());

  setSingleSubscription(_domainName, "Sonar");
}

void
SonarAvoid::action()
{
  // get parameters
  const SonarAvoidParameters * params =
    dynamic_cast<const SonarAvoidParameters *>(params_);
  // make sure nothing went wrong
  assert(params != NULL);

  // pointer to sonar event type
  const Miro::RangeBunchEventIDL * pScan;
  if (event->remainder_of_body >>= pScan) {

    // accumulate sensor bunch readings in internal buffer
    for (int i = pScan->sensor.length() - 1; i >= 0; --i) {
      sensor_[pScan->sensor[i].index] = pScan->sensor[i].range;
    }

    // search minimum fronal sonar value, to decide wether to avoid or not
    int i = params->leftSonar;
    int j = params->rightSonar;
    if (i > j)
      std::swap(i, j);

    int minimum = sensor_[i];
    int id = i;
    for (++i; i <= j; ++i) {
      if (sensor_[i] < minimum) {
	minimum = sensor_[i];
        id = i;
      }
    }

    // create motion arbiter messate
    Miro::BAP::MotionArbiterMessage message;

    // init arbiter message
    message.id = this;
    // if there is an obstacle, get active
    if (minimum < params->minDistance) {
      message.active = true;
      // if the obstacle is to close, halt
      message.velocity.translation = 
	(minimum < params->haltDistance)? 0 : params->translation;
      // turn away from obstacle
      message.velocity.rotation = 
	(std::abs(static_cast<int>(params->leftSonar - id)) > 
	 std::abs(static_cast<int>(params->rightSonar - id)))?
	params->rotation : -params->rotation;
    }
    else
      message.active = false;
    
    arbitrate(message);
  }
  else {
    // Range Sensor Event type mismatch !
    std::cerr << "SonarAvoid: No RangeBunchEventIDL message." << std::endl;
  }
}

const std::string&
SonarAvoid::getBehaviourName() const
{
  return name_;
}
