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
#ifndef DummyBehaviourParameters_h
#define DummyBehaviourParameters_h

#include "miro/TimedBehaviourParameters.h"

struct DummyBehaviourParameters : public Miro::TimedBehaviourParameters
{
  int  dummy;

  DummyBehaviourParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;

private:
  typedef Miro::TimedBehaviourParameters Super;
};
#endif
