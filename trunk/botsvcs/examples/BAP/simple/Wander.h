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
#ifndef Wander_h
#define Wander_h

#include "Parameters.h"

#include "botsvcs/BAP/TimedBehaviour.h"
#include "botsvcs/BAP/MotionArbiterMessage.h"

class Wander : public Miro::BAP::TimedBehaviour 
{
  typedef Miro::BAP::TimedBehaviour Super;

public:
  Wander(ACE_Reactor &);

  BEHAVIOUR_PARAMETERS_FACTORY(WanderParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  Miro::BAP::MotionArbiterMessage message_;

  static const std::string name_;
};

#endif
