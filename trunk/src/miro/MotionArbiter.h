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
#ifndef MotionArbiter_h
#define MotionArbiter_h

#include "MotionC.h"
#include "PriorityArbiter.h"
#include "MotionArbiterMessage.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <string>

namespace Miro
{
  // forward declarations
  class StructuredPushSupplier;
  class MotionArbiterMessage;

  //! Priority based Arbiter for the generalized Motion interface.
  class MotionArbiter : public PriorityArbiter
  {
    //! Super class type.
    typedef PriorityArbiter Super;

  public:
    MotionArbiter(Motion_ptr _pMotion, StructuredPushSupplier * _pSupplier = NULL);

    //! Factory method for ArbiterMessage
    MotionArbiterMessage * getMessageInstance();

    virtual void close();
    virtual const std::string& getName() const;

  protected:
    virtual void setActuators(const ArbiterMessage& _message);
    virtual void limpActuators();

    Motion_ptr pMotion_;
    StructuredPushSupplier * pSupplier_;

    VelocityIDL currentVelocity_;
    CosNotification::StructuredEvent notifyEvent;

    //! Arbiter name.
    static const std::string name_;
  };
};
#endif
