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

#include "Parameters.h"
#include "DummyBehaviourD.h"
#include "DummyArbiterMessage.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

unsigned long DummyBehaviourD::counter_ = 0;

const std::string DummyBehaviourD::name_ = "DummyBehaviourD";

DummyBehaviourD::DummyBehaviourD(const std::string& _transition) :
  Super(*ACE_Reactor::instance()),
  transition_(_transition)
{
  std::cout << "Constructing " << name_ << "." << endl;
}

DummyBehaviourD::~DummyBehaviourD()
{
  std::cout << "Destructing " << name_ << "." << endl;
}

void
DummyBehaviourD::action()
{
  std::cout << name_ << " action: " << counter_ << endl;

  DummyArbiterMessage message;
  message.id = this;
  message.active = false;
  message.message = "Action of " + name_;
  arbitrate(message);

  Miro::BehaviourParameters * const p0 = 
    getSuccessorBehaviourParameters(transition_, "Behaviour0");
  DummyBehaviourParameters * const p =
    dynamic_cast<DummyBehaviourParameters * const>(p0);

  p->dummy *= 3;

  setSuccessorBehaviourParameters(transition_, "Behaviour0", p);
}

const std::string&
DummyBehaviourD::getBehaviourName() const
{
  return name_;
}
