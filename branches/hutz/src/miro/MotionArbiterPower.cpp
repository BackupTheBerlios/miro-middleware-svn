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


#include "MotionArbiterPower.h"
#include "MotionArbiterMessage.h"
#include "MotionArbiterParameters.h"
#include "Behaviour.h"
#include "IO.h"
#include "StructuredPushSupplier.h"

using std::string;

namespace Miro
{
  const string MotionArbiterPower::name_ = "MotionArbiterPower";

  MotionArbiterPower::MotionArbiterPower(SparrowMotion_ptr _pMotion, StructuredPushSupplier * _pSupplier) :
    scale_( 20.0 ),
    offset_( 8000.0 ),
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
  MotionArbiterPower::getParametersInstance()
  {
    return new MotionArbiterParameters();
  }

  void
  MotionArbiterPower::init(const ArbiterParameters * _params)
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
      double leftPower;
      double rightPower;
      leftPower = velocity.translation + velocity.rotation;
      leftPower *= scale_;
      if( leftPower < 0 )
	leftPower -= offset_;
      else if( leftPower > 0 )
	leftPower += offset_;
      rightPower = velocity.translation - velocity.rotation;
      rightPower *= scale_;
      if( rightPower < 0 )
	rightPower -= offset_;
      else if( rightPower > 0 )
	rightPower += offset_;
      pMotion_->setLRPower((int)leftPower, (int)rightPower);

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
  MotionArbiterPower::arbitrate(const ArbiterMessage& _message)
  {
    const MotionArbiterMessage& message =
      dynamic_cast<const MotionArbiterMessage &>(_message);

    if (active_) {
      Guard guard(mutex_);
      MotionArbiterParameters::PriorityMap::const_iterator i
	= params_->priorities.find(message.id);
      if (i != params_->priorities.end()) {
	if (message_[i->second].active != message.active)
	  cout << "MotionArbiterPower: " << message.id->getBehaviourName() 
	       << " - " << message.active << endl;
	message_[i->second] = message;
      
	setMotion();
      }
      else {
	cerr << "MotionArbiterPower: got message from unregistered behaviour: "
	     << message.id->getBehaviourName() << endl;
      }
    }
    else {
      cerr << "MotionArbiterPower: got arbitrate call while not active from behaviour: "
	   << message.id->getBehaviourName() << endl;
    }
  }


  void
  MotionArbiterPower::setMotion()
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
	  cout << "MotionArbiterPower: " << flush << behaviourName 
	       << " -  set velocity: " << flush << j->velocity << endl;
	  pMotion_->setVelocity(j->velocity);
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
  MotionArbiterPower::getName() const
  {
    return name_;
  }
};
