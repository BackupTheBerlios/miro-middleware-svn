// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef ConstraintArbiter_h
#define ConstraintArbiter_h

#include "idl/DifferentialMotionC.h"
#include "PriorityArbiter.h"
#include "ConstraintArbiterViewerTask.h"
#include "VelocitySpace.h"
#include "MotionArbiterMessage.h"
#include "ConstraintArbiterParameters.h"
#include "TimeSeries.h"
#include "Synch.h"

#include <ace/Reactor.h>

#include <stdio.h>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;

  typedef MotionArbiterMessage ConstraintArbiterMessage;

  class ConstraintArbiter :
    public PriorityArbiter,
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
    virtual ConstraintArbiterMessage * getMessageInstance() const;
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
#endif
