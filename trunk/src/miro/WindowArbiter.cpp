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

//#include "ActionPattern.h"
#include "ArbiterParameters.h"
#include "ArbiterMessage.h"
#include "WindowArbiter.h"
#include "WindowArbiterViewer.h"
#include "Behaviour.h"
#include "StructuredPushSupplier.h"

#include <complex>
#include <cmath>

namespace Miro
{
  const std::string WindowArbiter::name_ = "WindowArbiter";

  WindowArbiter::WindowArbiter(ACE_Reactor &ar_, 
			       DifferentialMotion_ptr _pMotion,
			       StructuredPushSupplier * _pSupplier) :
    pMotion_(DifferentialMotion::_duplicate(_pMotion)),
    pSupplier_(_pSupplier),
    reactor(ar_),
    timerId(0),
    dynWindow_(std::complex<double>(0., 0.), 10000, 10000),
    dynWindowFinished_(std::complex<double>(0.,0. ), 10000, 10000),
    winArbViewTask_(NULL),
    winArbViewTaskCreated(false)
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

  WindowArbiter::~WindowArbiter() {
    if(winArbViewTaskCreated) {
      delete winArbViewTask_;
      winArbViewTaskCreated = false; 
    }
  }

#ifdef WE_ACTUALLY_NEED_IT
  void
  WindowArbiter::init(const ArbiterParameters * _params)
  {
    Arbiter::init(_params);

    leftVelocity_ = 0;
    rightVelocity_ = 0;
  }
#endif

  void
  WindowArbiter::calcActivation()
  {
    // search whether there is an active behaviour
    MessageVector::const_iterator first, last = message_.end();
    for (first = message_.begin(); first != last; ++first) {
      if ((*first)->active) {
	break;
      }
    }

    // if none is active we should better stop
    if (first == last) {
      pMotion_->limp();
    }
  }

  void
  WindowArbiter::open()
  {	
    // super class open
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
    //    ActionPattern * ap = ActionPattern::currentActionPattern();
    //    ActionPattern::BehaviourMap bm = ap->behaviourMap();
        
    // Let each behaviour calculate its dynamicWindow-entrys (ascend by priority)
    ArbiterParameters::RegistrationMap::const_iterator j;
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
    if (velocity.translation != currentVelocity_.translation ||
	velocity.rotation != currentVelocity_.rotation) {
      pMotion_->setVelocity(velocity);
      currentVelocity_ = velocity;
    }
    return 0;
  }
  
  const std::string&
  WindowArbiter::getName() const
  {
    return name_;
  }
};
