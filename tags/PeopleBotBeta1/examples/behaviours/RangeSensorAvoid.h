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
#ifndef RangeSensorAvoid_h
#define RangeSensorAvoid_h

#include "RangeSensorBehaviour.h"
#include "RangeSensorAvoidParameters.h"

class RangeSensorAvoid : public RangeSensorBehaviour
{
  typedef RangeSensorBehaviour Super;

public:
  RangeSensorAvoid(Miro::Client& _client,
		   CosNotifyChannelAdmin::EventChannel_ptr _ec,
		   const std::string& _name,
		   const std::string& _domainName,
		   Miro::StructuredPushSupplier * _pSupplier = NULL);

  // factory method for BehaviourParameters
  RangeSensorAvoidParameters * getParametersInstance();
  void action();

protected:
  double evalScanMin(double alpha, double beta);
};
#endif
