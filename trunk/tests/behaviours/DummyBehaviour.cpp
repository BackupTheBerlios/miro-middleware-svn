// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DummyBehaviour.h"
#include "DummyArbiterMessage.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(DummyBehaviour, DummyBehaviourParameters)

DummyBehaviour::DummyBehaviour(const std::string& _name) :
  Super(*ACE_Reactor::instance()),
  active_(false),
  name_(_name)
{
  std::cout << "Constructing " << name_ << "." << endl;
}

DummyBehaviour::~DummyBehaviour()
{
  std::cout << "Destructing " << name_ << "." << endl;
}

void
DummyBehaviour::action()
{
  DummyBehaviourParameters const * params =
    dynamic_cast<DummyBehaviourParameters const * >(params_);

  std::cout << name_ << " action: " << active_ << endl;

  if (params->dummy != 0)
    std::cout << name_ << " dummy: " << params->dummy << endl;

  DummyArbiterMessage message;
  message.id = this;
  message.active = active_;
  message.message = "Action of " + name_;

  arbitrate(message);

  active_ = !active_;
}

const std::string&
DummyBehaviour::getBehaviourName() const
{
  return name_;
}
