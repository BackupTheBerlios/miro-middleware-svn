// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
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
#include "TimeHelper.h"
#include "Log.h"

#include "idl/ArbiterEventC.h"

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
    timerId(-1)
  {
    MIRO_LOG_CTOR("Miro::ConstraintArbiter");
    
    currentVelocity_.translation = 0;
    currentVelocity_.rotation = 0.;

    if (pSupplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name =
      CORBA::string_dup(pSupplier_->domainName().c_str());
        notifyEvent_.header.fixed_header.event_type.type_name =
      CORBA::string_dup("ConstraintArbiter");
      notifyEvent_.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent_.header.variable_header.length(0);   // put nothing here
      notifyEvent_.filterable_data.length(0);          // put nothing here

      // offer declaration
      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent_.header.fixed_header.event_type;

      StructuredPushSupplier::IndexVector v =
	pSupplier_->addOffers(offers);

      offerIndex_ = v[0];
    }
  }

  ConstraintArbiter::~ConstraintArbiter()
  {
  }

  ConstraintArbiterParameters *
  ConstraintArbiter::getParametersInstance() const
  {
    ConstraintArbiterParameters * p = new ConstraintArbiterParameters();
    // set the wheelbase
    // this shouldn't change for different action patterns
    // but we have to deferr this initialization until runtime
    p->velocitySpace.setWheelBase(pMotion_->getWheelBase());

    return p;
  }

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
  ConstraintArbiter::init(ArbiterParameters const * _params)
  {
    PriorityArbiter::init(_params);

    ConstraintArbiterParameters const * params =
      dynamic_cast<ConstraintArbiterParameters const *>(params_);

    MIRO_ASSERT(params != NULL);

    if (timerId != -1)
      reactor.reset_timer_interval(timerId, params->pace);
  }

  void
  ConstraintArbiter::open()
  {
    // open arbiter
    Arbiter::open();

    ConstraintArbiterParameters const * params =
      dynamic_cast<ConstraintArbiterParameters const *>(params_);

    MIRO_ASSERT(params != NULL);

    // init timer for calculating velocity space every 50000usecs
    timerId = reactor.schedule_timer(this, 0,
				     ACE_Time_Value(0,0),
				     params->pace);

    // debug message
    MIRO_DBG_OSTR(MIRO,LL_NOTICE, "ConstraintArbiter.cpp : open()" << std::endl);
  }

  void
  ConstraintArbiter::close()
  {
    // debug message
    MIRO_DBG_OSTR(MIRO,LL_CTOR_DTOR, "ConstraintArbiter.cpp : close()" << std::endl);

    // release timer
    if (timerId != -1) {
      reactor.cancel_timer(timerId);
      timerId = -1;
    }

    // stop robot
    pMotion_->setLRVelocity(0, 0);

    // close arbiter
    Arbiter::close();
  }

  int
  ConstraintArbiter::handle_timeout(const ACE_Time_Value &, const void*)
  {
    ConstraintArbiterParameters * params =
      const_cast<ConstraintArbiterParameters *>
      ( dynamic_cast<ConstraintArbiterParameters const *>(params_));
    MIRO_ASSERT(params != NULL);
    Vector2d velocity;
    //    std::FILE *logFile1;

    ACE_Time_Value start = ACE_OS::gettimeofday();

    typedef std::vector<Behaviour *> BehaviourVector;

    // get the registered behaviours ascenting by priority
    BehaviourVector bv(params_->priorities.size());
    ArbiterParameters::RegistrationMap::const_iterator j;
    for(j = params_->priorities.begin(); j != params_->priorities.end(); j++) {
      bv[j->second] = j->first;
    }
	 
    // preinitialize the velocity space
    params->velocitySpace.clearAllEvals();
    params->velocitySpace.clearCurvatureConstraints();
	 
    // let each behaviour calculate its velocity space
    BehaviourVector::const_iterator k;
    for(k = bv.begin(); k != bv.end(); k++) {
      (*k)->addEvaluation(&params->velocitySpace);
    }
	 
    // calculate new velocity using the content of the velocity space
    velocity = params->velocitySpace.applyObjectiveFunctionToEval();
	 
    //    cout << "LEFT: " << velocity.real() << " ::: " << velocity.imag() << endl;

    // set steering commands
    pMotion_->setLRVelocity(velocity.real(), velocity.imag());

    ACE_Time_Value stop = ACE_OS::gettimeofday();

    MIRO_DBG_OSTR(MIRO, LL_PRATTLE, "ConstraintArbiter eval time: " << stop - start);

//    logFile1 = std::fopen("velocityspace.log","a");
//    for(int r_index = 0; r_index <= 2*(velocitySpace_.maxVelocity_/velocitySpace_.spaceResolution_)+1; r_index++) {
//	for(int l_index = 0; l_index <= 2*(velocitySpace_.maxVelocity_/velocitySpace_.spaceResolution_)+1; l_index++) {
//		fprintf(logFile1, "%d\n", velocitySpace_.velocitySpace_[l_index][r_index]);
//	}
//    }
//    fclose(logFile1);

    if (pSupplier_ &&
	pSupplier_->subscribed(offerIndex_)) {

      unsigned int const headerSize = 12;
      unsigned int dataSize = params->velocitySpace.dynamicWindowSize();

      unsigned char * buffer = new unsigned char[headerSize + dataSize];
      short * h = reinterpret_cast<short *>(buffer);
      h[0] = 0; // protocol id
      h[1] = params->velocitySpace.size();
      params->velocitySpace.windowBounds(h[2], h[3], h[4], h[5]);
      
      unsigned char * p = buffer + headerSize;
      params->velocitySpace.dumpDynamicWindow(p);
      Miro::VelocitySpaceIDL * dynamicWindow = 
	new Miro::VelocitySpaceIDL(headerSize + dataSize,
				   headerSize + dataSize, buffer, 1);

      notifyEvent_.remainder_of_body <<= dynamicWindow;
      pSupplier_->sendEvent(notifyEvent_);
    }

    return 0;
  }

  const std::string&
  ConstraintArbiter::getName() const
  {
    return name_;
  }

  /** Dummy implemenation as we do it the other way round. */
  void 
  ConstraintArbiter::setActuators(const ArbiterMessage&)
  {

  }
   

  void 
  ConstraintArbiter::limpActuators()
  {
    // set steering commands
    pMotion_->setLRVelocity(0, 0);
  }
}
