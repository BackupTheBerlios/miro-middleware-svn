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
#ifndef sonarAvoid_hh
#define sonarAvoid_hh

#include "miro/RangeSensorC.h"
#include "miro/EventBehaviour.h"

#include "RangeSensorAvoidParameters.h"

#include <deque>

namespace Miro
{
  class Client;
};

class RangeSensorAvoid : public Miro::EventBehaviour
{
  typedef Miro::EventBehaviour Super;
  typedef std::deque<double> SensorScan;

public:
  RangeSensorAvoid(Miro::Client& _client,
		   CosNotifyChannelAdmin::EventChannel_ptr _ec,
		   const std::string& _name,
		   const std::string& _domainName);

  // factory method for BehaviourParameters
  RangeSensorAvoidParameters * getParametersInstance();

  void init(const Miro::BehaviourParameters * _params);
  void action();
  const std::string& getBehaviourName() const;

protected:
  void addBuffer(SensorScan& _scan, double _range);
  void evalSensor(unsigned long group, unsigned long index, long range);

  Miro::Client& client_;
  Miro::RangeSensor_var rangeSensor_;

  Miro::Mutex mutex_;

  SensorScan left_;
  SensorScan right_;
  SensorScan leftFront_;
  SensorScan rightFront_;
  SensorScan front_;

  const std::string name_;
  const std::string domainName_;
  std::string sensorName_;

  Miro::ScanDescriptionIDL_var description_;
};
#endif
