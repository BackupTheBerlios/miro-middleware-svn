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
#ifndef WallFollow_h
#define WallFollow_h

#include "RangeSensorBehaviour.h"
#include "WallFollowParameters.h"

class WallFollow : public RangeSensorBehaviour
{
  typedef RangeSensorBehaviour Super;

public:
  WallFollow(Miro::Client& _client,
	     CosNotifyChannelAdmin::EventChannel_ptr _ec,
	     const std::string& _name,
	     const std::string& _domainName,
	     Miro::StructuredPushSupplier * _pSupplier = NULL);

  // factory method for BehaviourParameters
  WallFollowParameters * getParametersInstance();

  void action();

protected:
  bool regressionsGerade(const SensorScan& _scan, double delta,
			 double& m, double& b) const;
};
#endif