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
#include "SonarAvoid.h"

#include "miro/RangeEventC.h"
#include "miro/MotionArbiterMessage.h"

const std::string SonarAvoid::name_ = "SonarAvoid";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(SonarAvoid, SonarAvoidParameters)

SonarAvoid::SonarAvoid(Miro::RangeSensor_ptr _rangeSensor,
		       CosNotifyChannelAdmin::EventChannel_ptr _ec,
		       const string& _domainName) :
  Super(_ec),
  domainName_(_domainName),
  sensor_()
{
  cout << "Constructing SonarAvoid behaviour." << endl;

  // set up the sensor scan buffer
  Miro::ScanDescriptionIDL_var scanDescription = 
    _rangeSensor->getScanDescription();
  if (scanDescription->group.length() > 0)
    sensor_.resize(scanDescription->group[0].sensor.length());

  // subscribe for sonar events
  CosNotification::EventTypeSeq added;
  CosNotification::EventTypeSeq removed;
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName_.c_str());
  added[0].type_name = CORBA::string_dup("Sonar");

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumer.consumerAdmin_->subscription_change(added, removed);
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
    Miro::MotionArbiterMessage message;

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
    std::cerr << "SonarAvoid: No RangeBunchEventIDL message." << endl;
  }
}

const string&
SonarAvoid::getBehaviourName() const
{
  return name_;
}
