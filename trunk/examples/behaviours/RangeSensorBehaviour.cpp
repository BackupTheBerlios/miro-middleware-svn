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
#include "RangeSensorBehaviour.h"

#include "miro/MotionStatusC.h"
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

RangeSensorBehaviour::RangeSensorBehaviour(Miro::Client& _client,
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
  cout << "Constructing RangeSensorBehaviour behaviour." << endl;

  EventTypeSeq added(0);
  EventTypeSeq removed(1);
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName_.c_str());
  added[0].type_name = CORBA::string_dup("RawPosition");

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumer.consumerAdmin_->subscription_change(added, removed);
}

void
RangeSensorBehaviour::init(const Miro::BehaviourParameters * _params)
{
  Miro::Guard guard(mutex_);

  const RangeSensorBehaviourParameters * params =
    dynamic_cast<const RangeSensorBehaviourParameters *>(_params); 

  rangeSensor_ = client_.resolveName<RangeSensor>(params->sensorName.c_str());

  // set up the sensor scan buffer
  description_ = rangeSensor_->getScanDescription();

  cout << name_ << ": subscribing for " << description_->eventName << endl;

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
RangeSensorBehaviour::action()
{
  const Miro::RangeBunchEventIDL * pBunchScan;
  const Miro::RangeGroupEventIDL * pGroupScan;
  const Miro::RangeScanEventIDL * pScan;
  const Miro::RawPositionIDL * pPosition;

  cout << name_ << ": integrating data" << endl;

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
  else if (event->remainder_of_body >>= pPosition) {
    position_ = Vector2d(pPosition->position.point.x, pPosition->position.point.y);
    heading_ = pPosition->position.heading;
  }
  else {
    std::cerr << name_ << ": Unhandled Event!" << endl; 
  }
}

const string&
RangeSensorBehaviour::getBehaviourName() const
{
  return name_;
}

void
RangeSensorBehaviour::addBuffer(SensorScan& _scan, const Vector2d& _p)
{
  const RangeSensorBehaviourParameters * params =
    dynamic_cast<const RangeSensorBehaviourParameters *>(params_);

  // transform into a global coordinate frame
  // to integrate data over multiple robot positions

  Vector2d p(_p);
  p *= Vector2d(cos(heading_), sin(heading_));
  p += position_;

  if (_scan.size() > params->historySize) {
    _scan.pop_back();
  }
  _scan.push_front(p);
}

void 
RangeSensorBehaviour::evalSensor(unsigned long group, unsigned long index, long range)
{
  if (range == Miro::RangeSensor::INVALID_RANGE)
    return;

  const RangeSensorBehaviourParameters * params =
    dynamic_cast<const RangeSensorBehaviourParameters *>(params_);

  bool ignore = false;
  if (range == Miro::RangeSensor::HORIZON_RANGE)
    ignore = true;

  // calc egocentric sensor origin
  double d = description_->group[group].sensor[index].distance;
  Vector2d p(cos(description_->group[group].sensor[index].alpha) * d,
	     sin(description_->group[group].sensor[index].alpha) * d);

  // calc sensor heading
  double a = ( description_->group[group].sensor[index].alpha +
	       description_->group[group].sensor[index].beta);
  Vector2d q(cos(a) * (double)range, sin(a) * (double)range);

  // calc egocentric range end point;
  p += q;

  // left sensors
  if (a >= Miro::deg2Rad(90) - params->apexAngle && 
      a <= Miro::deg2Rad(90) +  params->apexAngle)
    if (!ignore)
      addBuffer(left_, p);
    else if (left_.size() > 0)
      left_.pop_back();
  // front left sensors
  if (a >= Miro::deg2Rad(45) - params->apexAngle && 
      a <= Miro::deg2Rad(45) +  params->apexAngle)
    if (!ignore)
      addBuffer(leftFront_, p);
    else if (leftFront_.size() > 0)
      leftFront_.pop_back();
  // front sensors
  if (a >= Miro::deg2Rad(0) - params->apexAngle && 
      a <= Miro::deg2Rad(0) +  params->apexAngle)
    if (!ignore)
      addBuffer(front_, p);
    else if (front_.size() > 0)
      front_.pop_back();
  // front right sensors
  if (a >= Miro::deg2Rad(-45) - params->apexAngle &&
      a <= Miro::deg2Rad(-45) +  params->apexAngle)
    if (!ignore)
      addBuffer(rightFront_, p);
    else if (rightFront_.size() > 0)
      rightFront_.pop_back();
  // right sensors
  if (a >= Miro::deg2Rad(-90) - params->apexAngle && 
      a <= Miro::deg2Rad(-90) +  params->apexAngle)
    if (!ignore)
      addBuffer(right_, p);
    else if (right_.size() > 0)
      right_.pop_back();
}

