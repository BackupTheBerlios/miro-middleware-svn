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
DummyBehaviour::init(Miro::BAP::BehaviourParameters const * _params) 
{
  DummyBehaviourParameters const * params =
    dynamic_cast<DummyBehaviourParameters const * >(_params);
  MIRO_ASSERT(params != NULL);

  sendTransition_ = params->sendTransition;

  Super::init(_params);
}

std::string
DummyBehaviour::actionTake2(Miro::BAP::BehaviourParameters const * _params,
			    Miro::BAP::ArbiterMessage * _message)
{
  DummyBehaviourParameters const * params =
    dynamic_cast<DummyBehaviourParameters const * >(_params);
  MIRO_ASSERT(params != NULL);
  DummyArbiterMessage * message =
    dynamic_cast<DummyArbiterMessage *>(_message);
  MIRO_ASSERT(message != NULL);

  std::cout << name_ << " action: " << active_ << endl;

  if (params->dummy != 0)
    std::cout << name_ << " dummy: " << params->dummy << endl;


  message->id = this;
  message->active = active_;
  message->message = "Action of " + name_;

  active_ = !active_;

  std::string transition;
  std::cout << name_ << " transition counter: " << sendTransition_ << std::endl;
  if (sendTransition_) {
    --sendTransition_;
    if (sendTransition_ == 0) {
      transition = std::string("Transition") + name_;
      std::cout << "Send transition: " << transition << std::endl;
    }
  }
  return transition;
}

const std::string&
DummyBehaviour::getBehaviourName() const
{
  return name_;
}
