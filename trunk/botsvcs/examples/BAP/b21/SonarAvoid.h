// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
<<<<<<< SonarAvoid.h
// (c) 2001, 2002, 2003, 2004
=======
// (c) 2000, 2001, 2002, 2003, 2004
>>>>>>> 1.4
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef SonarAvoid_h
#define SonarAvoid_h

#include "idl/RangeSensorC.h"
#include "botsvcs/BAP/EventBehaviour.h"

#include "Parameters.h"

#include <vector>

class SonarAvoid : public Miro::BAP::EventBehaviour
{
  typedef Miro::BAP::EventBehaviour Super;

public:
  SonarAvoid(Miro::RangeSensor_ptr _rangeSensor,
	     CosNotifyChannelAdmin::EventChannel_ptr _ec,
	     const std::string& _domainName);

  BEHAVIOUR_PARAMETERS_FACTORY(SonarAvoidParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  typedef std::vector<long> SensorScan;

  SensorScan sensor_;

  static const std::string name_;
};
#endif
