// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PriorityArbiter_h
#define PriorityArbiter_h

#include "Arbiter.h"
#include "miro/Parameters.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <vector>

namespace Miro
{
  // forward declarations
  class Behaviour;

  typedef ArbiterMessage PriorityArbiterMessage;

  //! Priority Arbiter base class.
  class PriorityArbiter : public Arbiter
  {
  public:
    //! Default constructor.
    PriorityArbiter();

    ARBITER_TYPES_FACTORY(PriorityArbiter);

    virtual void calcActivation();
    //! Get the next active arbitration message with lower priority.
    virtual ArbiterMessage * getMessageForBehaviour(Behaviour * _id);

  protected:
    //! Pass the arbitration message to the actuators.
    virtual void setActuators(const ArbiterMessage& _message) = 0;
    //! Limp all actuators.
    virtual void limpActuators() = 0;

    //! The behaviour tha has currently control over the actuators.
    const Behaviour * currentBehaviour_;
  };
}
#endif
