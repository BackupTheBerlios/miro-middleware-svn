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

#include "MotionArbiter.h"
#include "Behaviour.h"
#include "StructuredPushSupplier.h"
#include "Parameters.h"

namespace Miro
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
    pMotion_(_pMotion),
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
      notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent.header.variable_header.length(0);   // put nothing here
      notifyEvent.filterable_data.length(0);          // put nothing here
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
};
