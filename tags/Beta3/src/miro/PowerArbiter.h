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
#ifndef PowerArbiter_h
#define PowerArbiter_h

#include "idl/SparrowMotionC.h"
#include "PriorityArbiter.h"
#include "PowerArbiterMessage.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declarations
  class StructuredPushSupplier;

  class PowerArbiter : public PriorityArbiter
  {
    //! Super class type.
    typedef PriorityArbiter Super;

  public:
    PowerArbiter(SparrowMotion_ptr _pMotion, 
		 StructuredPushSupplier * _pSupplier = NULL);

    // factory method for ArbiterParameters
    ARBITER_TYPES_FACTORY(PowerArbiter);

    virtual const std::string& getName() const;

  protected:
    virtual void setActuators(const ArbiterMessage& _message);
    virtual void limpActuators();

    //! Pointer to the motion interface.
    SparrowMotion_var pMotion_;
    //! Pointer to the supplier.
    StructuredPushSupplier * pSupplier_;

    //! The last issued left power.
    long currentLeft_;
    //! The last issued left power.
    long currentRight_;
    //! The last issued motion command.
    VelocityIDL currentVelocity_;

    //! Preconfigured event to send to the supplier.
    CosNotification::StructuredEvent notifyEvent;

    //! Arbiter name.
    static const std::string name_;
  };
};
#endif
