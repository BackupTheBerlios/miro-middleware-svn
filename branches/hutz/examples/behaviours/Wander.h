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
#ifndef Wander_h
#define Wander_h

#include "miro/TimedBehaviour.h"
#include "miro/MotionArbiterMessage.h"

#include "WanderParameters.h"

class Wander : public Miro::TimedBehaviour 
{
  typedef Miro::TimedBehaviour Super;

public:
  Wander(ACE_Reactor &);

  // factory method for BehaviourParameters
  WanderParameters * getParametersInstance();

  void action();
  const std::string& getBehaviourName() const;

protected:
  Miro::MotionArbiterMessage message_;

  static const std::string name_;
};

#endif
