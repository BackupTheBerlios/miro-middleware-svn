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
#ifndef RangeSensorWallFollow_h
#define RangeSensorWallFollow_h

#include "RangeSensorBehaviour.h"
#include "Parameters.h"

class RangeSensorWallFollow : public RangeSensorBehaviour
{
  typedef RangeSensorBehaviour Super;

public:
  RangeSensorWallFollow(Miro::Client& _client,
	     CosNotifyChannelAdmin::EventChannel_ptr _ec,
	     const std::string& _name,
	     const std::string& _domainName,
	     Miro::StructuredPushSupplier * = NULL);

  BEHAVIOUR_PARAMETERS_FACTORY(RangeSensorWallFollowParameters);
  void action();

protected:
  bool regressionsGerade(double _alpha, double _beta, double _delta,
			 double& m, double& b) const;
};
#endif
