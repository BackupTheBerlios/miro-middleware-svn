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
#ifndef TurnRightParameters_h
#define TurnRightParameters_h

#include "miro/TimedBehaviourParameters.h"

struct TurnRightParameters : public Miro::TimedBehaviourParameters
{
  int minDistance;
  int haltDistance;
  unsigned int leftLaser;
  unsigned int rightLaser;
  int translation;
  double rotation;

  TurnRightParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;

private:
  typedef Miro::TimedBehaviourParameters Super;
};
#endif
