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

#include "PowerArbiter.h"
#include "PowerArbiterMessage.h"
#include "Behaviour.h"
#include "StructuredPushSupplier.h"

namespace Miro
{
  const std::string PowerArbiter::name_ = "PowerArbiter";

  PowerArbiter::PowerArbiter(SparrowMotion_ptr _pMotion, StructuredPushSupplier * _pSupplier) :
    pMotion_(_pMotion),
    pSupplier_(_pSupplier),
    currentLeft_(0),
    currentRight_(0)
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

  PowerArbiterMessage * 
  PowerArbiter::getMessageInstance()
  {
    return new PowerArbiterMessage();
  }

  void
  PowerArbiter::setActuators(const ArbiterMessage& _message)
  {
    const PowerArbiterMessage& message = dynamic_cast<const PowerArbiterMessage&>(_message);

    // flag to indicate wheter something changed 
    // (for debug output)
    bool newPower = false;
    
    // to keep the controller board traffic low
    // only send command, when anything changed
    if (currentBehaviour_ == NULL ||
	message.left != currentLeft_ ||
	message.right != currentRight_) {
      
      newPower = true;
      
      currentLeft_ = message.left;
      currentRight_ = message.right;
      pMotion_->setLRPower(currentLeft_, currentRight_);
      
      // velocity for debug output
      currentVelocity_.translation = (currentLeft_ + currentRight_) / 2;
    }

    // send arbitrate change debug output
    // but only if something changed
    if (pSupplier_ && (newPower || currentBehaviour_ != message.id)) {
      notifyEvent.header.fixed_header.event_name = 
	CORBA::string_dup(message.id->getBehaviourName().c_str());
      notifyEvent.remainder_of_body <<= currentVelocity_;
      pSupplier_->sendEvent(notifyEvent);
    }
  }
  
  void 
  PowerArbiter::limpActuators()
  {
    if (currentBehaviour_ != NULL) {
      // as the goalie misuses the lr power call,
      // we actually halt instead of limping
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
  PowerArbiter::getName() const
  {
    return name_;
  }
};
