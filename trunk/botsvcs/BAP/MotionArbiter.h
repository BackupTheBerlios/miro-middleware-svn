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
#ifndef BAP_MotionArbiter_h
#define BAP_MotionArbiter_h

#include "idl/MotionC.h"
#include "PriorityArbiter.h"
#include "MotionArbiterMessage.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declarations
  class StructuredPushSupplier;

  namespace BAP
  {
    //! Priority based Arbiter for the generalized Motion interface.
    class MotionArbiter : public PriorityArbiter
    {
      //! Super class type.
      typedef PriorityArbiter Super;

    public:
      //! Initializing construtor.
      MotionArbiter(Motion_ptr _pMotion,
		    ::Miro::StructuredPushSupplier * _pSupplier = NULL);
      
      ARBITER_TYPES_FACTORY(MotionArbiter);
      
      virtual void close();
      virtual const std::string& getName() const;

    protected:
      virtual void setActuators(const ArbiterMessage& _message);
      virtual void limpActuators();
      
      //! Pointer to the motion interface.
      Motion_var pMotion_;
      //! Pointer to the supplier.
      StructuredPushSupplier * pSupplier_;
      
      //! The last issued motion command.
      VelocityIDL currentVelocity_;
      //! Preconfigured event to send to the supplier.
      CosNotification::StructuredEvent notifyEvent;
      
      //! Arbiter name.
      static const std::string name_;
    };
  }
}
#endif
