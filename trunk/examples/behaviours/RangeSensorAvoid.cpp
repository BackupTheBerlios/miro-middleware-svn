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
#include "RangeSensorAvoid.h"

#include "miro/RangeSensorC.h"
#include "miro/RangeEventC.h"
#include "miro/Client.h"
#include "miro/MotionArbiterMessage.h"
#include "miro/Angle.h"

#include <algorithm>

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

RangeSensorAvoid::RangeSensorAvoid(Miro::Client& _client,
				   EventChannel_ptr _ec,
				   const string& _name,
				   const string& _domainName) :
  Super(_ec),
  client_(_client),
  name_(_name),
  domainName_(_domainName),
  sensorName_(),
  description_()
{
  cout << "Constructing RangeSensorAvoid behaviour." << endl;

  EventTypeSeq added(0);
  EventTypeSeq removed(1);
  added.length(0);
  removed.length(1);

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumer.consumerAdmin_->subscription_change(added, removed);
}

RangeSensorAvoidParameters *
RangeSensorAvoid::getParametersInstance()
{
  return new RangeSensorAvoidParameters();
}

void
RangeSensorAvoid::init(const Miro::BehaviourParameters * _params)
{
  Miro::Guard guard(mutex_);

  const RangeSensorAvoidParameters * params =
    dynamic_cast<const RangeSensorAvoidParameters *>(_params); 

  rangeSensor_ = client_.resolveName<RangeSensor>(params->sensorName.c_str());

  // set up the sensor scan buffer
  description_ = rangeSensor_->getScanDescription();

cout << "subscribing for " << description_->eventName << endl;

  EventTypeSeq added(1);
  EventTypeSeq removed(1);
  added.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName_.c_str());
  added[0].type_name = CORBA::string_dup(description_->eventName);

  if (sensorName_.length() > 0) {
    removed.length(1);
    removed[0].domain_name =  CORBA::string_dup(domainName_.c_str());
    removed[0].type_name = CORBA::string_dup(sensorName_.c_str());
  }
  
  consumer.consumerAdmin_->subscription_change(added, removed);

  Super::init(_params);
}

void
RangeSensorAvoid::action()
{
  Miro::Guard guard(mutex_);

  const RangeSensorAvoidParameters * params =
    dynamic_cast<const RangeSensorAvoidParameters *>(params_);
  const Miro::RangeBunchEventIDL * pBunchScan;
  const Miro::RangeGroupEventIDL * pGroupScan;
  const Miro::RangeScanEventIDL * pScan;

cout << "RangeSensorAvoid: integrating data" << endl;

  if (event->remainder_of_body >>= pBunchScan) {
    for (int i = pBunchScan->sensor.length() - 1; i >= 0; --i) {
      evalSensor(pBunchScan->sensor[i].group,
		 pBunchScan->sensor[i].index,
		 pBunchScan->sensor[i].range);
    }
  }
  else if (event->remainder_of_body >>= pGroupScan) {
    for (int i = pGroupScan->range.length() - 1; i >= 0; --i) {
      evalSensor(pGroupScan->group,
		 i,
		 pGroupScan->range[i]);
    }
  }
  else if (event->remainder_of_body >>= pScan) {
    for (int j = pScan->range.length() - 1; j >= 0; --j) {
      for (int i = pScan->range[j].length() - 1; i >= 0; --i) {
	evalSensor(j, i, pScan->range[j][i]);
      }
    }
  }
  else {
    std::cerr << "RangeSensorAvoid: No range sensor message" << std::endl;
    return;
  }

cout << "RangeSensorAvoid: evaluating data" << endl;

  // get the minimum distance measurement

 double minLeft = params->lateralDistance * 10.;
 double minLeftFront = params->minDistance * 10.;
 double minFront = params->minDistance * 10.;
 double minRightFront = params->minDistance * 10.;
 double minRight = params->lateralDistance * 10.;
 
 if (left_.size() > 0)
   minLeft = * min_element(left_.begin(), left_.end());
 if (leftFront_.size() > 0)
  minLeftFront = * min_element(leftFront_.begin(), leftFront_.end());
 if (front_.size() > 0)
  minFront = * min_element(front_.begin(), front_.end());
 if (rightFront_.size() > 0)
  minRightFront = * min_element(rightFront_.begin(), rightFront_.end());
 if (right_.size() > 0)
  minRight = * min_element(right_.begin(), right_.end());

  // build default message;
  MotionArbiterMessage message;
  message.id = this;
  message.active = false;
  message.velocity.translation = params->translation;
  message.velocity.rotation = 0;

cout << "RangeSensorAvoid: selecting action" << endl;

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

cout << "RangSensorAvoid: action." << endl;	   
  arbitrate(message);
}

const string&
RangeSensorAvoid::getBehaviourName() const
{
  return name_;
}

void
RangeSensorAvoid::addBuffer(SensorScan& _scan, double _range)
{
  const RangeSensorAvoidParameters * params =
    dynamic_cast<const RangeSensorAvoidParameters *>(params_);

  if (_range < 0.) {
    if(_scan.size() > 0) {
      _scan.pop_back();
    }
  }
  else {
    if (_scan.size() > params->historySize) {
      _scan.pop_back();
    }
    _scan.push_front(_range);
  }
}

void 
RangeSensorAvoid::evalSensor(unsigned long group, unsigned long index, long range)
{
  if (range == Miro::RangeSensor::INVALID_RANGE)
    return;

  const RangeSensorAvoidParameters * params =
    dynamic_cast<const RangeSensorAvoidParameters *>(params_);

  // calc sensor heading
  Miro::Angle a ((double) 
    ( description_->group[group].sensor[index].alpha +
      description_->group[group].sensor[index].beta ));

  // scale sensor to
  // double r = (double) range * cos(a);

  // left sensors
  if (a >= Miro::deg2Rad(90) - params->apexAngle && a <= Miro::deg2Rad(90) +  params->apexAngle)
    addBuffer(left_, range);
  // front left sensors
  if (a >= Miro::deg2Rad(45) - params->apexAngle && a <= Miro::deg2Rad(45) +  params->apexAngle)
    addBuffer(leftFront_, range);
  // front sensors
  if (a >= Miro::deg2Rad(0) - params->apexAngle && a <= Miro::deg2Rad(0) +  params->apexAngle)
    addBuffer(front_, range);
  // front right sensors
  if (a >= Miro::deg2Rad(-45) - params->apexAngle && a <= Miro::deg2Rad(-45) +  params->apexAngle)
    addBuffer(rightFront_, range);
  // right sensors
  if (a >= Miro::deg2Rad(-90) - params->apexAngle && a <= Miro::deg2Rad(-90) +  params->apexAngle)
    addBuffer(right_, range);
}

