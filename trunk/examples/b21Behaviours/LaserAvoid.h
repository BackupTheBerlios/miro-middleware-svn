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
#ifndef LaserAvoid_h
#define LaserAvoid_h

#include "miro/RangeSensorC.h"
#include "miro/TimedBehaviour.h"

#include "LaserAvoidParameters.h"

class LaserAvoid : public Miro::TimedBehaviour
{
  typedef Miro::TimedBehaviour Super;

public:
  LaserAvoid(ACE_Reactor& _reactor, Miro::RangeSensor_ptr _laser);

  BEHAVIOUR_PARAMETERS_FACTORY(LaserAvoidParameters);

  void action();
  const std::string& getBehaviourName() const;

  static const std::string name_;

protected:
  Miro::RangeSensor_var laser_;
};
#endif
