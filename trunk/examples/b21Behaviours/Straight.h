// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Straight_h
#define Straight_h

#include "miro/TimedBehaviour.h"

#include "StraightParameters.h"

class Straight : public Miro::TimedBehaviour 
{
  typedef Miro::TimedBehaviour Super;

public:
  Straight(ACE_Reactor &);

  // factory method for BehaviourParameters
  StraightParameters * getParametersInstance();

  void action();
  const std::string& getBehaviourName() const;

  static const std::string name_;
};

#endif
