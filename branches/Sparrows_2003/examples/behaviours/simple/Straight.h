// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Straight_h
#define Straight_h

#include "Parameters.h"

#include "miro/TimedBehaviour.h"

class Straight : public Miro::TimedBehaviour 
{
  typedef Miro::TimedBehaviour Super;

public:
  Straight(ACE_Reactor &);

  BEHAVIOUR_PARAMETERS_FACTORY(StraightParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  static const std::string name_;
};

#endif
