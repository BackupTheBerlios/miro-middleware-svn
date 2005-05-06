// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef LaserAvoid_h
#define LaserAvoid_h

#include "idl/RangeSensorC.h"
#include "botsvcs/BAP/TimedBehaviour.h"

#include "Parameters.h"

class LaserAvoid : public Miro::BAP::TimedBehaviour
{
  typedef Miro::BAP::TimedBehaviour Super;

public:
  LaserAvoid(ACE_Reactor& _reactor, Miro::RangeSensor_ptr _laser);

  BEHAVIOUR_PARAMETERS_FACTORY(LaserAvoidParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  Miro::RangeSensor_var laser_;

  static const std::string name_;
};
#endif
