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

#ifndef ConstraintArbiter_h
#define ConstraintArbiter_h

#include "idl/DifferentialMotionC.h"
#include "Arbiter.h"
#include "ConstraintArbiterViewerTask.h"
#include "VelocitySpace.h"

#include <ace/Reactor.h>

#include <stdio.h>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;

  class ConstraintArbiter :
    public Arbiter,
    public ACE_Event_Handler
  {
  public:
    ConstraintArbiter(ACE_Reactor &,
		  DifferentialMotion_ptr _pMotion,
		  StructuredPushSupplier * _pSupplier = NULL);
    ~ConstraintArbiter();

    virtual const std::string& getName() const;

    void open();
    void close();

    virtual int handle_timeout(const ACE_Time_Value&, const void*);

  protected:
    virtual void calcActivation();

    DifferentialMotion_var pMotion_;
    StructuredPushSupplier * pSupplier_;

    FILE *fileHandler;

    ACE_Reactor &reactor;
    int timerId;

    VelocityIDL currentVelocity_;

    VelocitySpace velocitySpace_, velocitySpacePaint_;

    ConstraintArbiterViewerTask * conArbViewTask_;
    bool conArbViewTaskCreated;

    CosNotification::StructuredEvent notifyEvent;

    static const std::string name_;
  };
};
#endif
