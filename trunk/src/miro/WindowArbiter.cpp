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
    dynWindow_(std::complex<double>(0., 0.), 700, 2000),
    dynWindowPaint_(std::complex<double>(0., 0.), 700, 2000),
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
    // open arbiter
    Arbiter::open();

    // create task for viewing dynamic window
    if(!winArbViewTaskCreated) {
      winArbViewTask_ = new WindowArbiterViewerTask(&dynWindowPaint_);
      winArbViewTask_->open();
      winArbViewTaskCreated = true;
    }

    // init timer for calculating dynamic window every 50000usecs
    timerId = reactor.schedule_timer(this, 0, ACE_Time_Value(0,0),
	ACE_Time_Value(0, 50000));

    // debug message
    std::cout << "WindowArbiter.cpp : open()" << std::endl;
  }

  void
  WindowArbiter::close()
  {
    // release timer
    reactor.cancel_timer(timerId);
    timerId = 0;

    // stop robot
    pMotion_->setLRVelocity(0, 0);

    // debug message
    std::cout << "WindowArbiter.cpp : close()" << std::endl;

    // close arbiter
    Arbiter::close();
  }

  int
  WindowArbiter::handle_timeout(const ACE_Time_Value &, const void*)
  {
    Miro::VelocityIDL  velocity;
    std::complex<double> newVelocity;

    // debug message
    // std::cout << "WindowArbiter.cpp : handle_timeout() started" << std::endl;

    // let each behaviour calculate its dynamicWindow ascend by priority
    typedef std::vector<Behaviour *> BehaviourVector;
    BehaviourVector bv(params_->priorities.size());
    ArbiterParameters::RegistrationMap::const_iterator j;
    for(j = params_->priorities.begin(); j != params_->priorities.end(); j++) {
      bv[j->second] = j->first;
    }
    BehaviourVector::const_iterator k;
    for(k = bv.begin(); k != bv.end(); k++) {
      (*k)->calcDynamicWindow(&dynWindow_);
    }

    // calculate new velocity using the content of the dynamicWindow
    newVelocity = dynWindow_.calcNewVelocity();

    // copy dynWindow for WindowArbiterViewer
    dynWindow_.getCopy(&dynWindowPaint_);

    // set motion
    pMotion_->setLRVelocity((int)(10. * newVelocity.real()), (int)(10. * newVelocity.imag()));
    currentVelocity_ = velocity;

    // std::cout << "WindowArbiter.cpp : handle_timeout() finished" << std::endl;

    return 0;
  }

  const std::string&
  WindowArbiter::getName() const
  {
    return name_;
  }
};
