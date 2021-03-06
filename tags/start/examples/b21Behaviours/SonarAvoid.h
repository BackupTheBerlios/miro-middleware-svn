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
#ifndef SonarAvoid_h
#define SonarAvoid_h

#include "miro/RangeSensorC.h"
#include "miro/EventBehaviour.h"

#include "SonarAvoidParameters.h"

#include <vector>

class SonarAvoid : public Miro::EventBehaviour
{
  typedef Miro::EventBehaviour Super;

public:
  SonarAvoid(Miro::RangeSensor_ptr _rangeSensor,
	     CosNotifyChannelAdmin::EventChannel_ptr _ec,
	     const std::string& _domainName);

  // factory method for BehaviourParameters
  SonarAvoidParameters * getParametersInstance();

  void action();
  const std::string& getBehaviourName() const;

protected:
  typedef std::vector<long> SensorScan;

  std::string domainName_;
  SensorScan sensor_;

  static const std::string name_;
};
#endif
