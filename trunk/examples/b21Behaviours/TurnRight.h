// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef TurnRight_h
#define TurnRight_h

#include "miro/RangeSensorC.h"
#include "miro/TimedBehaviour.h"

#include "TurnRightParameters.h"

class TurnRight : public Miro::TimedBehaviour
{
  typedef Miro::TimedBehaviour Super;

public:
  TurnRight(ACE_Reactor& _reactor, Miro::RangeSensor_ptr _laser);

  BEHAVIOUR_PARAMETERS_FACTORY(TurnRightParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  Miro::RangeSensor_var laser_;
};
#endif
