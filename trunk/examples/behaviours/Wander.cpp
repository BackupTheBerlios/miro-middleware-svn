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
#include "Wander.h"

#include <stdlib.h> 

using std::cout;
using std::string;

using Miro::MotionArbiterMessage;

const string Wander::name_ = "Wander";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(Wander, WanderParameters)

Wander::Wander(ACE_Reactor &ar_) : 
  Super(ar_)
{
  cout << "Constructing Wander behaviour." << endl;

  // initialise the arbitration message
  message_.id = this;
  message_.active = true;
  message_.velocity.translation = 10;
  message_.velocity.rotation = 0.;

  // initialise pseudo random numbers generator
  srand(ACE_OS::gettimeofday().usec());
}

void 
Wander::action() 
{
  const WanderParameters * params = 
    dynamic_cast<const WanderParameters *>(params_);

  double r = ((double)rand()) / ((double)RAND_MAX);
  if (r < params->thresholdChange) {
    r = ((double)rand()) / ((double)RAND_MAX);
    message_.velocity.translation = 
      params->minTranslation + 
      (long)(r * (params->maxTranslation - params->minTranslation));
    r = ((double)rand()) / ((double)RAND_MAX);
    if (r > params->thresholdStraight) {
      r = ((double)rand()) / ((double)RAND_MAX);
      message_.velocity.rotation = 
	- params->maxRotation + r * 2 * params->maxRotation;
    }
    else
      message_.velocity.rotation = 0.;
  }
  arbitrate(message_);
}

const string&
Wander::getBehaviourName() const 
{
  return name_;
}
