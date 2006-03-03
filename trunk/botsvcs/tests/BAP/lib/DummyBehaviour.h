// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef DummyBehaviour_h
#define DummyBehaviour_h

#include "botsvcs/tests/BAP/lib/Parameters.h"
#include "botsvcs/BAP/TimedBehaviour.h"

class DummyBehaviour : public Miro::BAP::TimedBehaviour
{
  typedef Miro::BAP::TimedBehaviour Super;

public:
  DummyBehaviour(const std::string& _name);
  virtual ~DummyBehaviour();

  BEHAVIOUR_PARAMETERS_FACTORY(DummyBehaviourParameters);
  virtual void init(Miro::BAP::BehaviourParameters const * _params);
  virtual std::string actionTake2(Miro::BAP::BehaviourParameters const * _params,
				  Miro::BAP::ArbiterMessage * _message);
  virtual const std::string& getBehaviourName() const;

protected:
  bool active_;
  int sendTransition_;
  
private:
  const std::string name_;
};

#endif
