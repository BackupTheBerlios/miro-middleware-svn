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
#ifndef DummyBehaviour_h
#define DummyBehaviour_h

#include "miro/TimedBehaviour.h"

class DummyBehaviour : public Miro::TimedBehaviour
{
public:
  DummyBehaviour(const std::string& _name);
  ~DummyBehaviour();

  void action();
  const std::string& getBehaviourName() const;

protected:
  bool active_;
  
private:
  const std::string name_;

  typedef Miro::TimedBehaviour Super;
};

#endif
