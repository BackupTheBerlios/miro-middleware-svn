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
#ifndef DummyBehaviourB_h
#define DummyBehaviourB_h

#include "miro/TimedBehaviour.h"

class DummyBehaviourB : public Miro::TimedBehaviour
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

  typedef Miro::TimedBehaviour Super;
};

#endif
