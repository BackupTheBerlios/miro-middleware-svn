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

#include "DummyBehaviourB.h"
#include "DummyArbiterMessage.h"

unsigned long DummyBehaviourB::counter_ = 0;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

DummyBehaviourB::DummyBehaviourB(const std::string& _name,
				 const std::string& _transition) :
  Super(*ACE_Reactor::instance()),
  transition_(_transition),
  name_(_name)
{
  std::cout << "Constructing " << name_ << "." << endl;
}

DummyBehaviourB::~DummyBehaviourB()
{
  std::cout << "Destructing " << name_ << "." << endl;
}

void
DummyBehaviourB::action()
{
  std::cout << name_ << " action: " << counter_ << endl;

  DummyArbiterMessage message;
  message.id = this;
  message.active = false;
  message.message = "Action of " + name_;
  arbitrate(message);

  if (!(++counter_ % 2))
    sendMessage(transition_);
}

const std::string&
DummyBehaviourB::getBehaviourName() const
{
  return name_;
}
