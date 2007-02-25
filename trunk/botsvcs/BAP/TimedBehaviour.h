// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
