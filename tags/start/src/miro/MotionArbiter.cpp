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


#include "MotionArbiter.h"
#include "MotionArbiterMessage.h"
#include "MotionArbiterParameters.h"
#include "Behaviour.h"
#include "IO.h"
#include "StructuredPushSupplier.h"

using std::string;

namespace Miro
{
  const string MotionArbiter::name_ = "MotionArbiter";

  MotionArbiter::MotionArbiter(Motion_ptr _pMotion, StructuredPushSupplier * _pSupplier) :
    pMotion_(_pMotion),
    pSupplier_(_pSupplier),
    mutex_(),
    params_(NULL),
    message_()
  {
    currentVelocity.translation = 0;
    currentVelocity.rotation = 0;

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

  MotionArbiterParameters * 
  MotionArbiter::getParametersInstance()
  {
    return new MotionArbiterParameters();
  }

  void
  MotionArbiter::init(const ArbiterParameters * _params)
  {
    Guard guard(mutex_);

    const MotionArbiterParameters * params
      = dynamic_cast<const MotionArbiterParameters *>(_params);

    // convert current arbitration to new configuration
    MessageVector tmp(message_);
    message_.resize(params->priorities.size()); // new message array size

    // if new configuration has no behaviour, that is allowed to arbitrate,
    // we better should stop
    if (message_.size() == 0) {
      Miro::VelocityIDL velocity;
      velocity.translation = 0;
      velocity.rotation = 0.;
      pMotion_->setVelocity(velocity);

      currentVelocity.translation = 0;
      currentVelocity.rotation = 0;
    }
    if (params_) { // allways except first time
      MotionArbiterParameters::PriorityMap::const_iterator i, j;
      // for each behaviour
      for (i = params->priorities.begin(); i != params->priorities.end(); ++i) {
	// if it existed before
	j = params_->priorities.find(i->first);
	if (j != params_->priorities.end()) {
	  // copy its current arbitration message to the new index
	  message_[i->second] = tmp[j->second];
	} 
	else {
	  // else reset entry
	  message_[i->second] = MotionArbiterMessage();
	}
      }
    }
    params_ = params;

    setMotion();
  }

  void
  MotionArbiter::arbitrate(const ArbiterMessage& _message)
  {
//    const MotionArbiterMessage& message =
//      dynamic_cast<const MotionArbiterMessage &>(_message);

    const ArbiterMessage& message = _message;


    if (active_) {
      Guard guard(mutex_);
      MotionArbiterParameters::PriorityMap::const_iterator i
	= params_->priorities.find(message.id);
      if (i != params_->priorities.end()) {
	if (message_[i->second].active != message.active)
	  cout << "MotionArbiter: " << message.id->getBehaviourName() 
	       << " - " << message.active << endl;
	message_[i->second] = message;
      
	setMotion();
      }
      else {
	cerr << "MotionArbiter: got message from unregistered behaviour: "
	     << message.id->getBehaviourName() << endl;
      }
    }
    else {
      cerr << "MotionArbiter: got arbitrate call while not active from behaviour: "
	   << message.id->getBehaviourName() << endl;
    }
  }


  void
  MotionArbiter::setMotion()
  {
    string behaviourName;

    // do the job
    MessageVector::const_iterator j;
    for (j = message_.begin(); j != message_.end(); ++j) {
      if (j->active) {
	// keep the can bus traffic low
	if (j->velocity.translation != currentVelocity.translation ||
	    j->velocity.rotation != currentVelocity.rotation) {

	  behaviourName = j->id->getBehaviourName();
	  //cout << "MotionArbiter: " << flush << behaviourName 
	  //     << " -  set velocity: " << flush << j->velocity << endl;

	  VelocityIDL velocity = j->velocity;

	  double fastWheel = velocity.translation + 225 * velocity.rotation;
	  if (fastWheel > 900)
	    velocity.translation = (int)(900 - 225 * velocity.rotation);
	    
	  pMotion_->setVelocity(velocity);
	  currentVelocity = j->velocity;
	}
	break;
      }
    }
    if (j == message_.end()) { // if nothing is active we should better stop
      cout << "limp" << endl;
      behaviourName = "None";
      currentVelocity.translation = 0;
      currentVelocity.rotation = 0;
      Miro::VelocityIDL velocity;
      velocity.translation = 0;
      velocity.rotation = 0.;
      pMotion_->setVelocity(velocity);
    }

    // send arbitrate change debug output
    if (pSupplier_) {
      notifyEvent.header.fixed_header.event_name = CORBA::string_dup(behaviourName.c_str());
      notifyEvent.remainder_of_body <<= currentVelocity;
      pSupplier_->sendEvent(notifyEvent);
    }
  }

  const string&
  MotionArbiter::getName() const
  {
    return name_;
  }
};
