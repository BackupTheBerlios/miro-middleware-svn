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
#ifndef RangeSensorBehaviourParameters_h
#define RangeSensorBehaviourParameters_h

#include "miro/EventBehaviourParameters.h"

#include <string>

struct RangeSensorBehaviourParameters : public Miro::EventBehaviourParameters
{
  unsigned int historyMSec;
  std::string sensorName;

  RangeSensorBehaviourParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;

private:
  typedef Miro::EventBehaviourParameters Super;
};
#endif
