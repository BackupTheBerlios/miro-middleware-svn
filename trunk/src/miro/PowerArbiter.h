// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PowerArbiter_h
#define PowerArbiter_h

#include "SparrowMotionC.h"
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
    PowerArbiterMessage * getMessageInstance();

    virtual const std::string& getName() const;

  protected:
    virtual void setActuators(const ArbiterMessage& _message);
    virtual void limpActuators();

    SparrowMotion_ptr pMotion_;
    StructuredPushSupplier * pSupplier_;

    long currentLeft_;
    long currentRight_;
    VelocityIDL currentVelocity_;
    CosNotification::StructuredEvent notifyEvent;

    //! Arbiter name.
    static const std::string name_;
  };
};
#endif
