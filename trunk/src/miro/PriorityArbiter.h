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
#ifndef PriorityArbiter_h
#define PriorityArbiter_h

#include "Arbiter.h"

#include <orbsvcs/CosNotifyCommC.h>

#include <vector>

namespace Miro
{
  // forward declarations
  class Behaviour;

  //! Priority Arbiter base class.
  class PriorityArbiter : public Arbiter
  {
  public:
    PriorityArbiter();

    virtual void calcActivation();

  protected:
    virtual void setActuators(const ArbiterMessage& _message) = 0;
    virtual void limpActuators() = 0;

    const Behaviour * currentBehaviour_;
  };
};
#endif
