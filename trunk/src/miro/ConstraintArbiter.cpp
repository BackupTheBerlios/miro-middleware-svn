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
#include "ConstraintArbiter.h"
#include "ConstraintArbiterViewer.h"
#include "Behaviour.h"
#include "StructuredPushSupplier.h"
#include "Log.h"

#include <complex>
#include <cmath>

namespace Miro
{
  const std::string ConstraintArbiter::name_ = "ConstraintArbiter";

  ConstraintArbiter::ConstraintArbiter(ACE_Reactor &ar_,
			       DifferentialMotion_ptr _pMotion,
			       StructuredPushSupplier * _pSupplier) :
    pMotion_(DifferentialMotion::_duplicate(_pMotion)),
    pSupplier_(_pSupplier),
    reactor(ar_),
    timerId(0),
    velocitySpace_(1000, 20, 2000, 500, 20),
    conArbViewTask_(NULL),
    conArbViewTaskCreated(false)
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

      // offer declaration
      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent.header.fixed_header.event_type;
      pSupplier_->addOffers(offers);
    }

  }

  ConstraintArbiter::~ConstraintArbiter() {
    if(conArbViewTaskCreated) {
      delete conArbViewTask_;
      conArbViewTaskCreated = false;
    }
  }

#ifdef WE_ACTUALLY_NEED_IT
  void
  ConstraintArbiter::init(const ArbiterParameters * _params)
  {
    Arbiter::init(_params);

    leftVelocity_ = 0;
    rightVelocity_ = 0;
  }
#endif

  void
  ConstraintArbiter::calcActivation()
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
  ConstraintArbiter::open()
  {
    // open arbiter
    Arbiter::open();

    // create task for viewing velocity space
    if(!conArbViewTaskCreated) {
      conArbViewTask_ = new ConstraintArbiterViewerTask(&velocitySpace_);
      conArbViewTask_->open();
      conArbViewTaskCreated = true;
    }

    // init timer for calculating velocity space every 50000usecs
    timerId = reactor.schedule_timer(this, 0, ACE_Time_Value(0,0),
	ACE_Time_Value(0, 50000));

    // debug message
    MIRO_DBG_OSTR(MIRO,LL_NOTICE, "ConstraintArbiter.cpp : open()" << std::endl);
  }

  void
  ConstraintArbiter::close()
  {
    // release timer
    reactor.cancel_timer(timerId);
    timerId = 0;

    // stop robot
    pMotion_->setLRVelocity(0, 0);

    // debug message
    MIRO_DBG_OSTR(MIRO,LL_CTOR_DTOR, "ConstraintArbiter.cpp : close()" << std::endl);

    // close arbiter
    Arbiter::close();
  }

  int
  ConstraintArbiter::handle_timeout(const ACE_Time_Value &, const void*)
  {
    std::complex<double> velocity;
    std::FILE *logFile1;

    // let each behaviour calculate its velocity space ascend by priority
    typedef std::vector<Behaviour *> BehaviourVector;
    BehaviourVector bv(params_->priorities.size());
    ArbiterParameters::RegistrationMap::const_iterator j;
    for(j = params_->priorities.begin(); j != params_->priorities.end(); j++) {
      bv[j->second] = j->first;
    }
    BehaviourVector::const_iterator k;
    for(k = bv.begin(); k != bv.end(); k++) {
      (*k)->addEvaluation(&velocitySpace_);
    }

    // calculate new velocity using the content of the velocity space
    velocity = velocitySpace_.applyObjectiveFunctionToEval();
    
    //    cout << "LEFT: " << velocity.real() << " ::: " << velocity.imag() << endl; 
    
    // set steering commands
    pMotion_->setLRVelocity(velocity.real(), velocity.imag());
    
//    logFile1 = std::fopen("velocityspace.log","a");
//    for(int r_index = 0; r_index <= 2*(velocitySpace_.maxVelocity_/velocitySpace_.spaceResolution_)+1; r_index++) {
//	for(int l_index = 0; l_index <= 2*(velocitySpace_.maxVelocity_/velocitySpace_.spaceResolution_)+1; l_index++) {
//		fprintf(logFile1, "%d\n", velocitySpace_.velocitySpace_[l_index][r_index]);
//	}
//    }
//    fclose(logFile1);
 
    return 0;
  }

  const std::string&
  ConstraintArbiter::getName() const
  {
    return name_;
  }
};
