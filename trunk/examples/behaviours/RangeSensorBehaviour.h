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
#ifndef RangeSensorBehaviour_h
#define RangeSensorBehaviour_h

#include "miro/RangeSensorC.h"
#include "miro/EventBehaviour.h"

#include "RangeSensorBehaviourParameters.h"

#include <complex>
#include <list>

namespace Miro
{
  class Client;
};

class RangeSensorBehaviour : public Miro::EventBehaviour
{
  typedef Miro::EventBehaviour Super;
protected:
  typedef std::complex<double> Vector2d;
  typedef std::list<Vector2d> SensorScan;

public:
  RangeSensorBehaviour(Miro::Client& _client,
		   CosNotifyChannelAdmin::EventChannel_ptr _ec,
		   const std::string& _name,
		   const std::string& _domainName);

  void init(const Miro::BehaviourParameters * _params);
  void action();
  const std::string& getBehaviourName() const;

protected:
  void addBuffer(SensorScan& _scan, const Vector2d& _p);
  void evalSensor(unsigned long group, unsigned long index, long range);

  Miro::Client& client_;
  Miro::RangeSensor_var rangeSensor_;

  Miro::Mutex mutex_;

  double heading_;
  Vector2d position_;

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
