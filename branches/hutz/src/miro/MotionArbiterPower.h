// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef MotionArbiterPower_h
#define MotionArbiterPower_h

#include "SparrowMotionC.h"
#include "Synch.h"
#include "Arbiter.h"
#include "MotionArbiterMessage.h"
#include "MotionArbiterParameters.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <vector>
#include <string>

namespace Miro
{
  class StructuredPushSupplier;

  class MotionArbiterPower : public Arbiter
  {
  public:
    MotionArbiterPower(SparrowMotion_ptr _pMotion, StructuredPushSupplier * _pSupplier = NULL);

    // factory method for ArbiterParameters
    MotionArbiterParameters * getParametersInstance();

    virtual void init(const ArbiterParameters * _parms);
    virtual void arbitrate(const ArbiterMessage& _message);
    virtual const std::string& getName() const;

    double scale_;
    double offset_;
  protected:
    void setMotion();

    typedef std::vector<MotionArbiterMessage> MessageVector;

    SparrowMotion_ptr pMotion_;
    StructuredPushSupplier * pSupplier_;

    Mutex mutex_;
    const MotionArbiterParameters * params_;
    MessageVector message_;

    VelocityIDL currentVelocity;
    CosNotification::StructuredEvent notifyEvent;

    static const std::string name_;
  };
};
#endif
