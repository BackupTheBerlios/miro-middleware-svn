// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "PriorityArbiter.h"
#include "ArbiterMessage.h"
#include "Parameters.h"

namespace Miro
{
  namespace BAP
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

    /**
     * @return A pointer containing an arbiter message.
     * Note that the caller takes ownership of the message and is
     * responsible for freeing its memory if the message is no longer
     * needed, by using delete.
     *
     * If no lower priority behaviour is active, the message returned is
     * a default constructed message with active == false.
     */
    ArbiterMessage *
    PriorityArbiter::getMessageForBehaviour(Behaviour * _id)
    {
      ArbiterMessage * message = getMessageInstance();
      
      Guard guard(mutex_);
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
      
      message->id = _id;
      return message;
    }
  }
}
