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
#ifndef TimedBehaviourParameters_h
#define TimedBehaviourParameters_h

#include <ace/OS.h>
#include "BehaviourParameters.h"

#include <iostream>


namespace Miro
{
  struct TimedBehaviourParameters : public BehaviourParameters
  {
    typedef BehaviourParameters Super;

    ACE_Time_Value pace;

    TimedBehaviourParameters();

    virtual void operator <<= (const KeyValueList&);

  protected:
    void printToStream(std::ostream& ostr) const;
  };
};
#endif
