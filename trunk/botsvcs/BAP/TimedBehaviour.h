// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BAP_TimedBehaviour_h
#define BAP_TimedBehaviour_h

#include "Behaviour.h"
#include "botsvcs/BAP/Parameters.h"

#include <ace/Reactor.h>

namespace Miro
{
  namespace BAP
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
      ~TimedBehaviour();

      BEHAVIOUR_PARAMETERS_FACTORY(TimedBehaviourParameters);
      
      virtual void open();
      virtual void close();
      virtual void init(const BehaviourParameters * _params);
      virtual int handle_timeout(const ACE_Time_Value&, const void*);

    protected:
      ACE_Reactor &reactor;
      int timerId;
    };
  }
}
#endif // BAP_TimedBehaviour_h
