// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef SonarAvoid_h
#define SonarAvoid_h

#include "idl/RangeSensorC.h"
#include "miro/EventBehaviour.h"

#include "Parameters.h"

#include <vector>

class SonarAvoid : public Miro::EventBehaviour
{
  typedef Miro::EventBehaviour Super;

public:
  SonarAvoid(Miro::RangeSensor_ptr _rangeSensor,
	     CosNotifyChannelAdmin::EventChannel_ptr _ec,
	     const std::string& _domainName);

  BEHAVIOUR_PARAMETERS_FACTORY(SonarAvoidParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  typedef std::vector<long> SensorScan;

  std::string domainName_;
  SensorScan sensor_;

  static const std::string name_;
};
#endif
