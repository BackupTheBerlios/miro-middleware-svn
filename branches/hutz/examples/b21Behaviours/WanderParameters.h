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
#ifndef WanderParameters_h
#define WanderParameters_h

#include "miro/TimedBehaviourParameters.h"

struct WanderParameters : public Miro::TimedBehaviourParameters
{
  double thresholdChange;
  double thresholdStraight;

  int minTranslation;
  int maxTranslation;
  double maxRotation;

  WanderParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;

private:
  typedef Miro::TimedBehaviourParameters Super;
};
#endif
