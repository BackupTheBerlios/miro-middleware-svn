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

#include <algorithm>

using std::cout;
using std::cerr;
using std::string;

using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;

using Miro::RangeSensor_ptr;
using Miro::ScanDescriptionIDL_var;
using Miro::MotionArbiterMessage;

const string SonarAvoid::name_ = "SonarAvoid";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(SonarAvoid, SonarAvoidParameters)

SonarAvoid::SonarAvoid(RangeSensor_ptr _rangeSensor,
		       EventChannel_ptr _ec,
		       const string& _domainName) :
  Super(_ec),
  domainName_(_domainName),
  sensor_()
{
  cout << "Constructing SonarAvoid behaviour." << endl;

  // set up the sensor scan buffer
  ScanDescriptionIDL_var scanDescription = 
    _rangeSensor->getScanDescription();
  if (scanDescription->group.length() > 0)
    sensor_.resize(scanDescription->group[0].sensor.length());

  EventTypeSeq added(1);
  EventTypeSeq removed(1);
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
  const Miro::RangeBunchEventIDL * pScan;
  const SonarAvoidParameters * params =
    dynamic_cast<const SonarAvoidParameters *>(params_);

  if (event->remainder_of_body >>= pScan) {

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

    //    int minimum = std::min_element(sensor_.begin() + i, sensor_.begin() + j + 1);
    // cout << "minimum: " << minimum << endl;

    MotionArbiterMessage message;

    message.id = this;
    if (minimum < params->minDistance) {
      message.active = true;
      message.velocity.translation = 
	(minimum < params->haltDistance)? 0 : params->translation;
      message.velocity.rotation = 
	(std::abs(static_cast<int>(params->leftSonar - id)) > 
	 std::abs(static_cast<int>(params->rightSonar - id)))?
	params->rotation : -params->rotation;
    }
    else
      message.active = false;
    
    arbitrate(message);
  }
  else
    cerr << "SonarAvoid: No RangeBunchEventIDL message." << endl;
}

const string&
SonarAvoid::getBehaviourName() const
{
  return name_;
}
