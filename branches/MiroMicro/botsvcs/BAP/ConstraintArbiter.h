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
#ifndef ConstraintArbiter_h
#define ConstraintArbiter_h

#include "idl/DifferentialMotionC.h"
#include "PriorityArbiter.h"
#include "ConstraintArbiterViewerTask.h"
#include "VelocitySpace.h"
#include "MotionArbiterMessage.h"
#include "ConstraintArbiterParameters.h"
#include "miro/TimeSeries.h"
#include "miro/Synch.h"

#include <ace/Reactor.h>

#include <stdio.h>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declarations;
  class StructuredPushSupplier;
  
  namespace BAP
  {
    typedef MotionArbiterMessage ConstraintArbiterMessage;
    
    class ConstraintArbiter : public PriorityArbiter, public ACE_Event_Handler
    {
    public:
      ConstraintArbiter(ACE_Reactor &,
			::Miro::DifferentialMotion_ptr _pMotion,
			::Miro::StructuredPushSupplier * _pSupplier = NULL);
      virtual ~ConstraintArbiter();
      
      //--------------------------------------------------------------------------
      // inherited Arbiter interface methods
      //--------------------------------------------------------------------------
      
      ARBITER_TYPES_FACTORY(ConstraintArbiter);
      virtual void init(ArbiterParameters const * _params);
      virtual void open();
      virtual void close();
      virtual const std::string& getName() const;
      
      //--------------------------------------------------------------------------
      // inherited ACE_Event_Handler interface methods
      //--------------------------------------------------------------------------
      
      virtual int handle_timeout(const ACE_Time_Value&, const void*);
      
    protected:
      //--------------------------------------------------------------------------
      // protected methods
      //--------------------------------------------------------------------------
      
      //! Pass the arbitration message to the actuators.
      virtual void setActuators(const ArbiterMessage& _message);
      //! Limp all actuators.
      virtual void limpActuators();
      
      
      //! Calcualte the current activattion of the velocity space.
      virtual void calcActivation();
      
      //! Reference to the differential motion interface.
      DifferentialMotion_var pMotion_;
      //! Pointer to the supplier for monitoring events.
      StructuredPushSupplier * pSupplier_;
      
      FILE *fileHandler;
      
      ACE_Reactor &reactor;
      int timerId;
      
      VelocityIDL currentVelocity_;
      
      CosNotification::StructuredEvent notifyEvent_;
      unsigned int offerIndex_;
      
      int skipDebug_;
      int skipMax_;
      
      TimeSeries<> timing_;
      
      //--------------------------------------------------------------------------
      // protected static data
      //--------------------------------------------------------------------------
      
      static const std::string name_;
    };
  }
}
#endif
