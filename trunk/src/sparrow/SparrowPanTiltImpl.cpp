// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "SparrowPanTiltImpl.h"
#include "SparrowConnection.h"
#include "SparrowConnection2003.h"

#include "miro/TimeHelper.h"
#include "miro/Angle.h"
#include "miro/StructuredPushSupplier.h"

namespace Sparrow
{
  using Miro::EDevIO;
  using Miro::ETimeOut;
  using Miro::EOutOfBounds;

  // Implementation skeleton constructor
  PanTiltImpl::PanTiltImpl(BaseConnection* _connection,
			   Miro::StructuredPushSupplier * _pSupplier) :
    Miro::PanImpl(Parameters::instance()->pan),
    connection(_connection),
    params_(Parameters::instance()->pan),
    pSupplier_(_pSupplier),
    lastPosition(Miro::deg2Rad(90.)),
    nextPosition(0.),
    timeLastSet(ACE_OS::gettimeofday()),
    totalLatency(params_.panLatency + params_.panSwing),
    sparrow2003_(Parameters::instance()->sparrow2003)
  {
    if (sparrow2003_) {
      ((Connection2003 *)connection)->setPan(Miro::deg2Rad(0.));
    }
    else {
      ((Connection *)connection)->setServo(0, Miro::deg2Rad(0.));
    }
    //    connection.setServo(1, params_.farAngle);

    // Stall Notify Event initialization
    if (pSupplier_) {
      notifyEvent.header.fixed_header.event_type.domain_name = 
        CORBA::string_dup(pSupplier_->domainName().c_str());
      notifyEvent.header.fixed_header.event_type.type_name = 
  	  CORBA::string_dup("Pan");
      notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent.header.variable_header.length(0);   // put nothing here
      notifyEvent.filterable_data.length(0);          // put nothing here

      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent.header.fixed_header.event_type;
      pSupplier_->addOffers(offers);
    }
  }

  // Implementation skeleton destructor
  PanTiltImpl::~PanTiltImpl()
  {
  }

  void
  PanTiltImpl::setPan(CORBA::Double _value) throw (EDevIO, EOutOfBounds)
  {
    Miro::Guard guard(mutex);

    if (_value != nextPosition) {

      // get current time
      ACE_Time_Value t = ACE_OS::gettimeofday();

      // set servo

      if(sparrow2003_){
         ((Connection2003 *)connection)->setPan(_value);
      }
      else{
         ((Connection *)connection)->setServo(0, _value);
      }
      // set positioning parameters
      lastPosition = currentPosition(t).angle;
      nextPosition = _value;
      setTargetPan(_value);
      timeLastSet = t;

      if (pSupplier_) {
	Miro::PanEventIDL panEvent;
	Miro::timeA2C(t, panEvent.time);
	panEvent.newAngle = _value;
	
	notifyEvent.remainder_of_body <<= panEvent;
	pSupplier_->sendEvent(notifyEvent);
      }
    }
  }

  CORBA::Double
  PanTiltImpl::getPan() throw (EDevIO)
  {
    Miro::Guard guard(mutex);
    ACE_Time_Value t = ACE_OS::gettimeofday();
    return currentPosition(t).angle;
  }

  CORBA::Boolean
  PanTiltImpl::panning(const Miro::TimeIDL& stamp) throw()
  {
    Miro::Guard guard(mutex);
    ACE_Time_Value t;
    Miro::timeC2A(stamp, t);
    return prvPanning(t);
  }

  Miro::PanPositionIDL
  PanTiltImpl::currentPan(const Miro::TimeIDL& stamp) throw()
  {
    Miro::Guard guard(mutex);
    ACE_Time_Value t;
    Miro::timeC2A(stamp, t);
    return currentPosition(t);
  }

  Miro::PanPositionIDL
  PanTiltImpl::currentPosition(const ACE_Time_Value& stamp)
  {
    Miro::PanPositionIDL position;

    if (params_.servo) {
      if (!prvPanning(stamp)) {
	// the pan doesn't move
	position.angle = (stamp < timeLastSet + params_.panLatency)? lastPosition : nextPosition;
	position.accuracy = params_.panAccuracy;
      }
      else {
	ACE_Time_Value t = stamp;
	t -= timeLastSet;
	t -= params_.panLatency;

	// estimated pan angle
	double alpha = t.usec();
	alpha /= 1000000.;
	alpha += t.sec();
	alpha *= params_.panRadPerSec;

	double delta = fabs(nextPosition - lastPosition);

	if (alpha > delta) {
	  // it is swing time
	  position.angle = nextPosition;
	  position.accuracy = params_.panSwingAccuracy;
	}
	else {
	  // we are panning
	  position.angle = (nextPosition > lastPosition)?
	    lastPosition + alpha : lastPosition - alpha;
	  position.accuracy = std::max(delta * .25, params_.panSwingAccuracy);
	}
      }
    }
    else {
      position.angle = ((Connection2003 *)connection)->getPanPosition();
      position.accuracy = params_.panAccuracy;
    }

    return position;
  }
}


