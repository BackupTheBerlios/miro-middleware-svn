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
#ifndef sonarAvoidParameters_hh
#define sonarAvoidParameters_hh

#include "miro/EventBehaviourParameters.h"

#include <string>

struct RangeSensorAvoidParameters : public Miro::EventBehaviourParameters
{
  unsigned int historySize;
  double apexAngle;

  int minDistance;
  int haltDistance;
  int lateralDistance;
  int translation;
  double rotation;

  std::string sensorName;

  RangeSensorAvoidParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;

private:
  typedef Miro::EventBehaviourParameters Super;
};
#endif
