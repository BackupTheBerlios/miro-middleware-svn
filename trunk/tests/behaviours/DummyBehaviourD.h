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
#ifndef DummyBehaviourC_h
#define DummyBehaviourC_h

#include "miro/TimedBehaviour.h"

class DummyBehaviourC : public Miro::TimedBehaviour
{
public:
  DummyBehaviourC(const std::string& _transition = "Transition");
  ~DummyBehaviourC();

  void action();
  const std::string& getBehaviourName() const;

protected:
  std::string transition_;

  static unsigned long counter_;
  
private:
  static const std::string name_;

  typedef Miro::TimedBehaviour Super;
};

#endif
