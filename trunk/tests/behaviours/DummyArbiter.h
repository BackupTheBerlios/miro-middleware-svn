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
#ifndef DummyArbiter_h
#define DummyArbiter_h

#include "miro/PriorityArbiter.h"
#include "DummyArbiterMessage.h"

class DummyArbiter : public Miro::PriorityArbiter
{
  //! Super class type.
  typedef PriorityArbiter Super;
  
public:
  DummyArbiter(const std::string& _name);
  
  // factory method for ArbiterParameters
  DummyArbiterMessage * getMessageInstance();
  
  virtual const std::string& getName() const;

protected:
  virtual void setActuators(const Miro::ArbiterMessage& _message);
  virtual void limpActuators();
  
  std::string message_;

  //! Arbiter name.
  const std::string name_;
};
#endif
