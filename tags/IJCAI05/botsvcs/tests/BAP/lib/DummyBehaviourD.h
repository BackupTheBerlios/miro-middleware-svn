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
#ifndef DummyBehaviourD_h
#define DummyBehaviourD_h

#include "botsvcs/BAP/TimedBehaviour.h"

class DummyBehaviourD : public Miro::BAP::TimedBehaviour
{
public:
  DummyBehaviourD(const std::string& _transition = "Transition");
  ~DummyBehaviourD();

  void action();
  const std::string& getBehaviourName() const;

protected:
  std::string transition_;

  static unsigned long counter_;
  
private:
  static const std::string name_;

  typedef Miro::BAP::TimedBehaviour Super;
};

#endif
