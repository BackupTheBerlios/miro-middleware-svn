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
#ifndef BAP_PriorityArbiter_h
#define BAP_PriorityArbiter_h

#include "Arbiter.h"
#include "botsvcs/BAP/Parameters.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <vector>

namespace Miro
{
  namespace BAP
  {
    // forward declarations
    class Behaviour;

    //! We do not define a priority arbiter message for now.
    typedef ArbiterMessage PriorityArbiterMessage;
    
    //! Priority Arbiter base class.
    class PriorityArbiter : public Arbiter
    {
    public:
      //! Default constructor.
      PriorityArbiter();
      
      ARBITER_TYPES_FACTORY(PriorityArbiter);
      
      //! Get the next active arbitration message with lower priority.
      virtual ArbiterMessage * getMessageForBehaviour(Behaviour * _id);
      virtual void calcActivation();
      
    protected:
      //! Pass the arbitration message to the actuators.
      virtual void setActuators(const ArbiterMessage& _message) = 0;
      //! Limp all actuators.
      virtual void limpActuators() = 0;
      
      //! The behaviour tha has currently control over the actuators.
      const Behaviour * currentBehaviour_;
    };
  }
}
#endif
