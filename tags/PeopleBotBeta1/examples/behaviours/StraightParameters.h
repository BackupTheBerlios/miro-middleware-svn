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
#ifndef StraightParameters_h
#define StraightParameters_h

#include "miro/TimedBehaviourParameters.h"

struct StraightParameters : public Miro::TimedBehaviourParameters
{
  int  translation;
  double rotation;

  StraightParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;

private:
  typedef Miro::TimedBehaviourParameters Super;
};
#endif
