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
#ifndef SonarAvoidParameters_h
#define SonarAvoidParameters_h

#include "miro/EventBehaviourParameters.h"

struct SonarAvoidParameters : public Miro::EventBehaviourParameters
{
  int minDistance;
  int haltDistance;
  unsigned int leftSonar;
  unsigned int rightSonar;
  int translation;
  double rotation;

  SonarAvoidParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;

private:
  typedef Miro::EventBehaviourParameters Super;
};
#endif
