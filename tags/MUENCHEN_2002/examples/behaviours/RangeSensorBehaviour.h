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
#ifndef RangeSensorBehaviour_h
#define RangeSensorBehaviour_h

#include "miro/RangeSensorC.h"
#include "miro/EventBehaviour.h"

#include "RangeSensorBehaviourParameters.h"

#include <complex>
#include <list>
#include <map>

namespace Miro
{
  class Client;
  class StructuredPushSupplier;
};

class RangeSensorBehaviour : public Miro::EventBehaviour
{
  typedef Miro::EventBehaviour Super;

protected:
  typedef std::complex<double> Vector2d;
  struct SensorReading
  {
    ACE_Time_Value time;
    Vector2d point;

    SensorReading(const ACE_Time_Value& _time, const Vector2d& _point) :
      time(_time),
      point(_point)
    {}
  };
  typedef std::list<SensorReading> SensorScan;
  typedef std::multimap<double, Vector2d> EgoMap;

public:
  RangeSensorBehaviour(Miro::Client& _client,
		       CosNotifyChannelAdmin::EventChannel_ptr _ec,
		       const std::string& _name,
		       const std::string& _domainName,
		       Miro::StructuredPushSupplier * _pSupplier = NULL);

  void init(const Miro::BehaviourParameters * _params);
  void action();
  const std::string& getBehaviourName() const;

protected:
  void truncateBuffer(const ACE_Time_Value& _time);
  void addBuffer(const ACE_Time_Value& _time, const Vector2d& _p);
  void evalSensor(const ACE_Time_Value& _time, 
		  unsigned long group, unsigned long index, long range);
  void evalScan();

  Miro::Client& client_;
  Miro::RangeSensor_var rangeSensor_;

  Miro::Mutex mutex_;

  bool initialized_;
  double heading_;
  Vector2d position_;

  SensorScan scan_;
  EgoMap egoMap_;

  const std::string name_;
  const std::string domainName_;
  std::string sensorName_;

  Miro::ScanDescriptionIDL_var description_;

  Miro::StructuredPushSupplier * pSupplier_;
};
#endif
