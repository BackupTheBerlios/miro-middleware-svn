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

#include "DummyArbiter.h"
#include "DummyArbiterMessage.h"

DummyArbiter::DummyArbiter(const std::string& _name) :
  message_(),
  name_(_name)
{
}

DummyArbiterMessage * 
DummyArbiter::getMessageInstance()
{
  return new DummyArbiterMessage();
}

void
DummyArbiter::setActuators(const Miro::ArbiterMessage& _message)
{
  const DummyArbiterMessage& message = dynamic_cast<const DummyArbiterMessage&>(_message);
   
  // only send output, when anything changed
  if (currentBehaviour_ == NULL ||
      message.message != message_) {
      
    std::cout << name_ << " has new arbitration:" << endl
	      << message << endl;
    message_ = message.message;
  }
}
  
void 
DummyArbiter::limpActuators()
{
  std::cout << name_ << ": no behaviour active." << endl;
}

const std::string&
DummyArbiter::getName() const
{
  return name_;
}
