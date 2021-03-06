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
#ifndef TimedBehaviour_h
#define TimedBehaviour_h

#include "Behaviour.h"
#include "TimedBehaviourParameters.h"

#include <ace/Reactor.h>

namespace Miro
{
  class TimedBehaviour : 
    public Behaviour, 
    public ACE_Event_Handler 
  {
    typedef Behaviour Super;
  public:
    TimedBehaviour (ACE_Reactor &);

    // factory method for BehaviourParameters
    TimedBehaviourParameters * getParametersInstance();

    void open();
    void close();
    void init(const BehaviourParameters * _params);
    virtual int handle_timeout(const ACE_Time_Value&, const void*);

  protected:
    ACE_Reactor &reactor;
    int timerId;
  };
};
#endif
