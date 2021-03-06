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
  std::cout << name_ << " action: " << active_ << endl;

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
