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
#ifndef RangeSensorAvoidParameters_h
#define RangeSensorAvoidParameters_h

#include "RangeSensorBehaviourParameters.h"

#include <string>

struct RangeSensorAvoidParameters : public RangeSensorBehaviourParameters
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
  typedef RangeSensorBehaviourParameters Super;
};
#endif
