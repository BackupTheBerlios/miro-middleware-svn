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


#include "ActionPattern.h"
#include "WindowArbiter.h"
#include "WindowArbiterViewer.h"
#include "WindowArbiterMessage.h"
#include "WindowArbiterParameters.h"
#include "Behaviour.h"
#include "IO.h"
#include "StructuredPushSupplier.h"
#include <complex>
#include <cmath>

using std::string;

namespace Miro
{
  const string WindowArbiter::name_ = "WindowArbiter";

  WindowArbiter::WindowArbiter(ACE_Reactor &ar_, Motion_ptr _pMotion,
	StructuredPushSupplier * _pSupplier) :
    pMotion_(_pMotion),
    pSupplier_(_pSupplier),
    mutex_(),
    params_(NULL),
    message_(),
    reactor(ar_),
    timerId(0),
    dynWindow_(std::complex<double>(0., 0.), 10000, 10000),
    dynWindowFinished_(std::complex<double>(0.,0. ), 10000, 10000),
    winArbViewTask_(NULL),
    winArbViewTaskCreated(false)
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

  WindowArbiter::~WindowArbiter() {
    if(winArbViewTaskCreated) {
      delete winArbViewTask_;
      winArbViewTaskCreated = false; 
    }
  }

  WindowArbiterParameters * 
  WindowArbiter::getParametersInstance()
  {
    return new WindowArbiterParameters();
  }

  void
  WindowArbiter::init(const ArbiterParameters * _params)
  {
    Guard guard(mutex_);

    leftVelocity_ = 0;
    rightVelocity_ = 0;

    const WindowArbiterParameters * params
      = dynamic_cast<const WindowArbiterParameters *>(_params);
    
    if(active_)
      reactor.reset_timer_interval(timerId, ACE_Time_Value(30, 0));
    
    leftVelocity_ = 0;
    rightVelocity_ = 0;
    
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
      WindowArbiterParameters::PriorityMap::const_iterator i, j;
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
	  message_[i->second] = WindowArbiterMessage();
	}
      }
    }
    params_ = params;

    setMotion();
  }

  void
  WindowArbiter::arbitrate(const ArbiterMessage& _message)
  {
    const WindowArbiterMessage& message =
      dynamic_cast<const WindowArbiterMessage &>(_message);

    if (active_) {
      Guard guard(mutex_);
      WindowArbiterParameters::PriorityMap::const_iterator i
	= params_->priorities.find(message.id);
      if (i != params_->priorities.end()) {
	if (message_[i->second].active != message.active)
	  cout << "WindowArbiter: " << message.id->getBehaviourName() 
	       << " - " << message.active << endl;
	message_[i->second] = message;
      
	setMotion();
      }
      else {
	cerr << "WindowArbiter: got message from unregistered behaviour: "
	     << message.id->getBehaviourName() << endl;
      }
    }
    else {
      cerr << "WindowArbiter: got arbitrate call while not active from behaviour: "
	   << message.id->getBehaviourName() << endl;
    }
  }


  void
  WindowArbiter::setMotion()
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
	  cout << "WindowArbiter: " << flush << behaviourName 
	       << " -  set velocity: " << flush << j->velocity << endl;

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

  void
  WindowArbiter::open()
  {	
    Arbiter::open();
    if(!winArbViewTaskCreated) {
      winArbViewTask_ = new WindowArbiterViewerTask(&dynWindowFinished_);
      winArbViewTask_->open();
      winArbViewTaskCreated = true; 
    }
      
    timerId = reactor.schedule_timer(this, 0, ACE_Time_Value(0,0),
	ACE_Time_Value(30, 0));
    cout << "WindowArbiter Open." << endl;
  }

  void
  WindowArbiter::close()
  {
    reactor.cancel_timer(timerId);
    timerId = 0;
    cout << "WindowArbiter Close." << endl;
    Arbiter::close();
  }	
  
  int
  WindowArbiter::handle_timeout(const ACE_Time_Value &, const void*)
  {
    const double RADSTAND = 390;

    Miro::VelocityIDL  velocity;
    std::complex<double> newVelocity;

    cout << "WindowArbiter TimeOutHandler." << endl;
    
    //    Miro::DynamicWindow dynWindow(std::complex<double>(leftVelocity_, rightVelocity_), MAX_POS_ACCEL, MAX_NEG_ACCEL);
    ActionPattern * ap = ActionPattern::currentActionPattern();
    ActionPattern::BehaviourMap bm = ap->behaviourMap();
        
    // Let each behaviour calculate its dynamicWindow-entrys (ascend by priority)
    WindowArbiterParameters::PriorityMap::const_iterator j;
    for(j = params_->priorities.begin(); j != params_->priorities.end(); j++) {
      if (j->first->isActive()) {
        j->first->calcDynamicWindow(&dynWindow_);
      }      
    }

    cout << "WindowArbiter All Operations Completed" << endl;

    // Calculate new velocity using the content of the dynamicWindow    
    //    newVelocity = dynWindow_.calcNewVelocity();
    //    newVelocity = std::complex<double>(std::max(std::min(50., newVelocity.real()),-50.), std::max(std::min(50.,newVelocity.imag()),-50.));
    //    dynWindow_.setNewDynamicWindow(newVelocity);


    // Create copy of the finished dynWindow for Debugging
    dynWindowFinished_.velocity_ = dynWindow_.velocity_;
    dynWindowFinished_.minLeft_ = dynWindow_.minLeft_;
    dynWindowFinished_.maxLeft_ = dynWindow_.maxLeft_;
    dynWindowFinished_.minRight_ = dynWindow_.minRight_;
    dynWindowFinished_.maxRight_ = dynWindow_.maxRight_;
    
    for(int left = dynWindow_.minLeft_; left <= dynWindow_.maxLeft_; left++) {
      for(int right = dynWindow_.minRight_; right <= dynWindow_.maxRight_; right++) {
	dynWindowFinished_.velocitySpace_[left+100][right+100] = dynWindow_.velocitySpace_[left+100][right+100];
      }
    }
    
    // Save actual Velocity
    leftVelocity_ = (int)newVelocity.real();
    rightVelocity_ = (int)newVelocity.imag();
    cout << "WindowArbiter --> LEFT_VELOCITY : " << leftVelocity_ << endl;
    cout << "WindowArbiter --> RIGHT_VELOCITY : " << rightVelocity_ << endl;
    
    // Set motion
    velocity.translation = 10 * (leftVelocity_ + rightVelocity_) / 2;
    velocity.rotation = 10 * (leftVelocity_ - rightVelocity_) / RADSTAND;
    if (velocity.translation != currentVelocity.translation ||
	velocity.rotation != currentVelocity.rotation) {
      pMotion_->setVelocity(velocity);
      currentVelocity = velocity;
    }
    return 0;
  }
  
  const string&
  WindowArbiter::getName() const
  {
    return name_;
  }
};
