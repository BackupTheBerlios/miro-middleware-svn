// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PriorityArbiter.h"
#include "ArbiterMessage.h"
#include "Parameters.h"

namespace Miro
{
  ARBITER_TYPES_FACTORY_IMPL(PriorityArbiter);

  PriorityArbiter::PriorityArbiter() :
    currentBehaviour_(NULL)
  {
  }

  void
  PriorityArbiter::calcActivation()
  {
    MessageVector::const_iterator first, last = message_.end();
    for (first = message_.begin(); first != last; ++first) {
      // find first active behaviours
      // as we do priority arbitration this is the one to go
      if ((*first)->active) {

	setActuators(**first);
	currentBehaviour_ = (*first)->id;
	
	break;
      }
    }

    // if nothing is active we should better stop
    if (first == last) {
      limpActuators();
      currentBehaviour_ = NULL;
    }
  }

  ArbiterMessage *
  PriorityArbiter::getInferior(Behaviour const * _id)
  {
    ArbiterMessage * message = getMessageInstance();

    MessageVector::const_iterator first, last = message_.end();
    for (first = message_.begin(); first != last; ++first) {
      // find first calling behaviours
      if ((*first)->id == _id) {
	break;
      }
    }
    
    // if this behaviour has a successor
    // get his arbitration
    if (first != last) {
      ++first;
      for (; first != last; ++first) {
	// find first active behaviour
	if ((*first)->active) {
	  message->assign(**first);
	  break;
	}
      }
    }

    return message;
  }
}
