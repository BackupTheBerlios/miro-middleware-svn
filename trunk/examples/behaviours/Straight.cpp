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
#include "Straight.h"

#include "miro/MotionArbiterMessage.h"

using std::cout;
using std::string;

using Miro::MotionArbiterMessage;

const string Straight::name_ = "Straight";

Straight::Straight(ACE_Reactor &ar_) : 
  Super(ar_)
{
  cout << "Constructing Straight behaviour." << endl;
}

StraightParameters *
Straight::getParametersInstance()
{
  return new StraightParameters();
}

void 
Straight::action() 
{
  const StraightParameters * params = 
    dynamic_cast<const StraightParameters *>(params_);
  MotionArbiterMessage message;

  message.id = this;
  message.active = true;
  message.velocity.translation = params->translation;
  message.velocity.rotation = params->rotation;

  arbitrate(message);
}

const string&
Straight::getBehaviourName() const 
{
  return name_;
}
