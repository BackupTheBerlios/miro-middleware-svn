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
#ifndef DummyBehaviourB_h
#define DummyBehaviourB_h

#include "botsvcs/BAP/TimedBehaviour.h"

class DummyBehaviourB : public Miro::BAP::TimedBehaviour
{
public:
  DummyBehaviourB(const std::string& _name,
		  const std::string& _transition = "Transition");
  ~DummyBehaviourB();

  void action();
  const std::string& getBehaviourName() const;

protected:
  std::string transition_;

  static unsigned long counter_;
  
private:
  const std::string name_;

  typedef Miro::BAP::TimedBehaviour Super;
};

#endif
