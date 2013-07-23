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
#include "MotionArbiter.h"
#include "Behaviour.h"
#include "Parameters.h"

#include "miro/StructuredPushSupplier.h"

namespace Miro
{
  namespace BAP
  {
    const std::string MotionArbiter::name_ = "MotionArbiter";
    
    ARBITER_TYPES_FACTORY_IMPL(MotionArbiter);
    
    /**
     * @param _pMotion a pointer to the motion interface
     * @param _pSupplier a pointer to a StructuredPushSupplier for
     *   emitting arbitration events for debugging.
     */
    MotionArbiter::MotionArbiter(Motion_ptr _pMotion, 
				 StructuredPushSupplier * _pSupplier) :
      pMotion_(Motion::_duplicate(_pMotion)),
      pSupplier_(_pSupplier)
    {
      currentVelocity_.translation = 0;
      currentVelocity_.rotation = 0.;
      
      if (pSupplier_) {
	// Status Notify Event initialization
	notifyEvent.header.fixed_header.event_type.domain_name = 
	  CORBA::string_dup(pSupplier_->domainName().c_str());
        notifyEvent.header.fixed_header.event_type.type_name = 
	  CORBA::string_dup("ArbitrateChange");

	CosNotification::EventTypeSeq offers;
	offers.length(1);
	offers[0] = notifyEvent.header.fixed_header.event_type;
	pSupplier_->addOffers(offers);
      }
    }
    
    void
    MotionArbiter::close()
    {
      Super::close();
      pMotion_->limp();
    }
    
    void
    MotionArbiter::setActuators(const ArbiterMessage& _message)
    {
      const MotionArbiterMessage& message = dynamic_cast<const MotionArbiterMessage&>(_message);
      
      // flag to indicate wheter something changed 
      // (for debug output)
      bool newVelocity = false;
      
      // to keep the controller board traffic low
      // only send command, when anything changed
      if (currentBehaviour_ == NULL ||
	  message.velocity.translation != currentVelocity_.translation ||
	  message.velocity.rotation != currentVelocity_.rotation) {
	
	newVelocity = true;
	
	pMotion_->setVelocity(message.velocity);
	currentVelocity_ = message.velocity;
      }
      
      
      // send arbitrate change debug output
      // but only if something changed
      if (pSupplier_ && (newVelocity || currentBehaviour_ != message.id)) {
	notifyEvent.header.fixed_header.event_name = 
	  CORBA::string_dup(message.id->getBehaviourName().c_str());
	notifyEvent.remainder_of_body <<= currentVelocity_;
	pSupplier_->sendEvent(notifyEvent);
      }
    }
    
    void 
    MotionArbiter::limpActuators()
    {
      if (currentBehaviour_ != NULL) {
	currentVelocity_.translation = 0;
	currentVelocity_.rotation = 0.;
	pMotion_->setVelocity(currentVelocity_);
	
	// send arbitrate change debug output
	if (pSupplier_) {
	  notifyEvent.header.fixed_header.event_name = CORBA::string_dup("None");
	  notifyEvent.remainder_of_body <<= currentVelocity_;
	  pSupplier_->sendEvent(notifyEvent);
	}
      }
    }
    
    const std::string&
    MotionArbiter::getName() const
    {
      return name_;
    }
  }
}
