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
#ifndef OnButtonParameters_h
#define OnButtonParameters_h

#include "miro/EventBehaviourParameters.h"

struct OnButtonParameters : public Miro::EventBehaviourParameters
{
  typedef EventBehaviourParameters Super;

  int button;
  bool onPress;

  OnButtonParameters();
  virtual void operator <<= (const Miro::KeyValueList& _params);

protected:
  void printToStream(std::ostream& ostr) const;
};
#endif
