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
#include "ConstraintArbiterParameters.h"
#include "Synch.h"

#include <ace/Reactor.h>

#include <stdio.h>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;

  typedef ArbiterMessage ConstraintArbiterMessage;

  class ConstraintArbiter :
    public Arbiter,
    public ACE_Event_Handler
  {
  public:
    ConstraintArbiter(ACE_Reactor &,
		  DifferentialMotion_ptr _pMotion,
		  StructuredPushSupplier * _pSupplier = NULL);
    virtual ~ConstraintArbiter();

    //--------------------------------------------------------------------------
    // inherited Arbiter interface methods
    //--------------------------------------------------------------------------

    virtual ConstraintArbiterParameters * 
    ConstraintArbiter::getParametersInstance() const;
    virtual void init(ArbiterParameters const * _params);
    virtual void open();
    virtual void close();
    virtual const std::string& getName() const;

    //--------------------------------------------------------------------------
    // inherited ACE_Event_Handler interface methods
    //--------------------------------------------------------------------------

    virtual int handle_timeout(const ACE_Time_Value&, const void*);

  protected:

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

    

    VelocitySpace velocitySpace_;
    ConstraintArbiterViewerTask * conArbViewTask_;
    bool conArbViewTaskCreated;


    CosNotification::StructuredEvent notifyEvent;

    static const std::string name_;
  };
}
#endif
