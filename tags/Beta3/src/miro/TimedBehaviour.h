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
#ifndef TimedBehaviour_h
#define TimedBehaviour_h

#include "Behaviour.h"
#include "Parameters.h"

#include <ace/Reactor.h>

namespace Miro
{
  class TimedBehaviour : 
    public Behaviour, 
    public ACE_Event_Handler 
  {
    //! Super class type.
    typedef Behaviour Super;
  public:
    //! Initializing constructor.
    TimedBehaviour (ACE_Reactor & _reactor);

    BEHAVIOUR_PARAMETERS_FACTORY(TimedBehaviourParameters);

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
