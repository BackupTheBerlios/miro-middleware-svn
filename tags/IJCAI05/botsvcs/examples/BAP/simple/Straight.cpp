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
#include "Straight.h"

#include "botsvcs/BAP/MotionArbiterMessage.h"
#include "miro/Log.h"

const std::string Straight::name_ = "Straight";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(Straight, StraightParameters)

Straight::Straight(ACE_Reactor &ar_) : 
  Super(ar_)
{
}

void 
Straight::action() 
{
  // get the parameters
  const StraightParameters * params = 
    dynamic_cast<const StraightParameters *>(params_);
  // make sure nothing went wrong
  MIRO_ASSERT(params != NULL);

  // create arbiter message
  Miro::BAP::MotionArbiterMessage message;

  // initialize arbiter message
  message.id = this;
  message.active = true;
  message.velocity.translation = params->translation;
  message.velocity.rotation = params->rotation;

  // send arbiter message
  arbitrate(message);
}

const std::string&
Straight::getBehaviourName() const 
{
  return name_;
}
