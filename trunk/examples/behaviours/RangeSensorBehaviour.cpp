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
#include "RangeSensorBehaviour.h"

#include "miro/MotionStatusC.h"
#include "miro/RangeSensorC.h"
#include "miro/RangeEventC.h"
#include "miro/Client.h"
#include "miro/Angle.h"
#include "miro/TimeHelper.h"

#include "miro/StructuredPushSupplier.h"
#include "nix/LineSamplesC.h"

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

int counter = 0;

RangeSensorBehaviour::RangeSensorBehaviour(Miro::Client& _client,
					   EventChannel_ptr _ec,
					   const string& _name,
					   const string& _domainName,
					   Miro::StructuredPushSupplier * _pSupplier) :
  Super(_ec),
  client_(_client),
  initialized_(false),
  name_(_name),
  domainName_(_domainName),
  sensorName_(),
  description_(),
  pSupplier_(_pSupplier)
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
  const Miro::RawPositionIDL * pPosition;
  const Miro::RangeBunchEventIDL * pBunchScan;
  const Miro::RangeGroupEventIDL * pGroupScan;
  const Miro::RangeScanEventIDL * pScan;

  const RangeSensorBehaviourParameters * params =
    dynamic_cast<const RangeSensorBehaviourParameters *>(params_);

  ACE_Time_Value timeStamp;

  //  cout << name_ << ": integrating data" << endl;

  if (event->remainder_of_body >>= pPosition) {
    position_ = Vector2d(pPosition->position.point.x, pPosition->position.point.y);
    heading_ = pPosition->position.heading;
    initialized_ = true;
  }
  else if (initialized_) {

    //    cout << name_ << ": integrating range senor data" << endl;

    ACE_Time_Value integrationTime;
    integrationTime.msec(params->historyMSec);
    
    truncateBuffer(integrationTime);

    if (event->remainder_of_body >>= pBunchScan) {
      Miro::timeC2A(pBunchScan->time, timeStamp);
      timeStamp = ACE_OS::gettimeofday();
      for (int i = pBunchScan->sensor.length() - 1; i >= 0; --i) {
	//	if (pBunchScan->sensor[i].group == 0)
	evalSensor(timeStamp,
		   pBunchScan->sensor[i].group,
		   pBunchScan->sensor[i].index,
		   pBunchScan->sensor[i].range);
      }
    }
    else if (event->remainder_of_body >>= pGroupScan) {
      Miro::timeC2A(pGroupScan->time, timeStamp);
      for (int i = pGroupScan->range.length() - 1; i >= 0; --i) {
	evalSensor(timeStamp,
		   pGroupScan->group,
		   i,
		   pGroupScan->range[i]);
      }
    }
    else if (event->remainder_of_body >>= pScan) {
      Miro::timeC2A(pScan->time, timeStamp);
      for (int j = pScan->range.length() - 1; j >= 0; --j) {
	for (int i = pScan->range[j].length() - 1; i >= 0; --i) {
	  evalSensor(timeStamp, j, i, pScan->range[j][i]);
	}
      }
    }
    else {
      std::cerr << name_ << ": Unhandled Event!" << endl; 
    }

    evalScan();
  }

#define SADF
#ifdef SADF
  if (pSupplier_ && !(++counter % 10)) {
    Nix::VisionFeaturesIDL * features = new Nix::VisionFeaturesIDL();

    //    cout << "map size: " << egoMap_.size() << endl;

    features->lineFeature.length(egoMap_.size());
    EgoMap::const_iterator j = egoMap_.begin();
    for (unsigned int i = 0; i < features->lineFeature.length(); ++i, ++j) {

      features->lineFeature[i].distance = abs(j->second);
      features->lineFeature[i].angle = arg(j->second);
    }
    features->rawPosition.point.x = position_.real();
    features->rawPosition.point.y = position_.imag();
    features->rawPosition.heading = heading_;

    features->worldPosition.point.x = position_.real();
    features->worldPosition.point.y = position_.imag();
    features->worldPosition.heading = heading_;
    features->cameraAngle = 0;

    features->goalFeature.visible = 0;
    features->goalFeature.post[0] = Nix::GoalItem::NOT_SEEN;
    features->goalFeature.post[1] = Nix::GoalItem::NOT_SEEN;

    features->cornerFeature.visible = 0;
    features->cornerFeature.post[0] = Nix::CornerItem::NOT_SEEN;
    features->cornerFeature.post[1] = Nix::CornerItem::NOT_SEEN;

    
    CosNotification::StructuredEvent lineEvent;

    lineEvent.header.fixed_header.event_type.domain_name =
      CORBA::string_dup(pSupplier_->domainName().c_str());
    lineEvent.header.fixed_header.event_type.type_name = 
      CORBA::string_dup("LineSamples");
    lineEvent.header.fixed_header.event_name = CORBA::string_dup("");
    lineEvent.header.variable_header.length(0);   // put nothing here
    lineEvent.filterable_data.length(0);          // put nothing here

    lineEvent.remainder_of_body <<= features;
    pSupplier_->sendEvent(lineEvent);
  } 
#endif
}

const string&
RangeSensorBehaviour::getBehaviourName() const
{
  return name_;
}

void
RangeSensorBehaviour::addBuffer(const ACE_Time_Value& _time, const Vector2d& _p)
{
//   const RangeSensorBehaviourParameters * params =
//     dynamic_cast<const RangeSensorBehaviourParameters *>(params_);

  // transform into a global coordinate frame
  // to integrate data over multiple robot positions

  Vector2d p(_p);
  p *= Vector2d(cos(heading_), sin(heading_));
  p += position_;

  scan_.push_back(SensorReading(_time, p));
}

void
RangeSensorBehaviour::truncateBuffer(const ACE_Time_Value& _delta)
{
  ACE_Time_Value t = ACE_OS::gettimeofday();

  SensorScan::iterator first, last = scan_.end();
  for (first = scan_.begin(); first != last; ++first)
    if (first->time + _delta >= t)
      break;

  scan_.erase(scan_.begin(), first);
}

void 
RangeSensorBehaviour::evalSensor(const ACE_Time_Value& _time,
				 unsigned long group, unsigned long index, long range)
{
  if (range == Miro::RangeSensor::INVALID_RANGE)
    return;

//   const RangeSensorBehaviourParameters * params =
//     dynamic_cast<const RangeSensorBehaviourParameters *>(params_);

  if (range == Miro::RangeSensor::HORIZON_RANGE)
    return;

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


  addBuffer(_time,p);
  //  cout << "abs(p)=" << abs(p) << "\targ(p)=" << Miro::rad2Deg(arg(p)) << "°" << endl;
}

void
RangeSensorBehaviour::evalScan()
{
  egoMap_.clear();

  // transform the global coordinate frame
  // to the egocentric mapping

  Vector2d alpha(cos(heading_), -sin(heading_));

  SensorScan::const_iterator first, last = scan_.end();
  for (first = scan_.begin(); first != last; ++first) {
    Vector2d p(first->point);
    p -= position_;
    p *= alpha;
    egoMap_.insert(std::make_pair(arg(p), p));
  }
}

