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
#ifndef WallFollowParameters_h
#define WallFollowParameters_h

#include "RangeSensorBehaviourParameters.h"

struct WallFollowParameters : public RangeSensorBehaviourParameters
{
  int minDistance;
  int maxDistance;

  WallFollowParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;

private:
  typedef RangeSensorBehaviourParameters Super;
};
#endif
