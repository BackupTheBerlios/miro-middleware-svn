// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
