// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003
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
    connection(_connection),
    params_(*Parameters::instance()),
    pSupplier_(_pSupplier),
    lastPosition(Miro::deg2Rad(90.)),
    nextPosition(0.),
    timeLastSet(ACE_OS::gettimeofday()),
    totalLatency(params_.panLatency + params_.panSwing),
    sparrow2003_(Parameters::instance()->sparrow2003)
  {
    if (sparrow2003_) {
      ((Connection2003 *)connection)->setServo(0, Miro::deg2Rad(0.));
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

#ifdef LETSTILTAGAIN
  Miro::PanTiltPositionIDL
  PanTiltImpl::getPosition() throw (EDevIO, ETimeOut)
  {
    Miro::Guard guard(mutex);

    return currentPosition();
  }

  void
  PanTiltImpl::setPosition(const PanTiltPositionIDL& dest)
    throw (EOutOfBounds, EDevIO)
  {
    Miro::Guard guard(mutex);

    if(sparrow2003_){
       ((Connection2003 *)connection)->setServo(0, dest.panvalue);
       ((Connection2003 *)connection)->setServo(1, dest.tiltvalue);
    }
    else{
       ((Connection *)connection)->setServo(0, dest.panvalue);
       ((Connection *)connection)->setServo(1, dest.tiltvalue);
    }

    timeLastSet = ACE_OS::gettimeofday();
    lastPosition = currentPosition();
    nextPosition = dest;
  }
#endif

  void
  PanTiltImpl::setPan(CORBA::Double value) throw (EDevIO, EOutOfBounds)
  {
    Miro::Guard guard(mutex);

    if (value != nextPosition) {

      // get current time
      ACE_Time_Value t = ACE_OS::gettimeofday();

      // set servo

      if(sparrow2003_){
         ((Connection2003 *)connection)->setServo(0, value);
      }
      else{
         ((Connection *)connection)->setServo(0, value);
      }
      // set positioning parameters
      lastPosition = currentPosition(t).angle;
      nextPosition = value;
      timeLastSet = t;

      if (pSupplier_) {
	Miro::PanEventIDL panEvent;
	Miro::timeA2C(t, panEvent.time);
	panEvent.newAngle = value;
	
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

  Miro::PanLimitsIDL 
  PanTiltImpl::getPanLimits() throw (EDevIO)
  {
    Miro::PanLimitsIDL result;
    result.minpanposition=-179.0f;
    result.maxpanposition=179.0f;
    
    return result;
  }

#ifdef LETSTILTAGAIN
  void
  PanTiltImpl::setTilt(CORBA::Double value) throw (EDevIO, EOutOfBounds)
  {
    Miro::Guard guard(mutex);


    if(sparrow2003_){
         ((Connection2003 *)connection)->setServo(1, value);
    }
    else{
         ((Connection *)connection)->setServo(1, value);
    }

    timeLastSet = ACE_OS::gettimeofday();
    lastPosition = currentPosition();
    nextPosition.tiltvalue = value;
  }

  CORBA::Double
  PanTiltImpl::getTilt() throw (EDevIO)
  {
    Miro::Guard guard(mutex);
    return currentPosition().tiltvalue;
  }

  Miro::TiltLimitsIDL 
  PanTiltImpl::getTiltLimits() throw (EDevIO)
  {
    Miro::TiltLimitsIDL result;
    result.mintiltposition=-90;
    result.maxtiltposition=90;
    
    return result;
  }

#endif


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

#ifdef LETSTILTAGAIN
  CORBA::Boolean
  PanTiltImpl::tilting() throw()
  {
    Miro::Guard guard(mutex);
    return prvTilting();
  }
#endif

  Miro::PanPositionIDL
  PanTiltImpl::currentPosition(const ACE_Time_Value& stamp)
  {
    Miro::PanPositionIDL position;

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

#ifdef LETSTILTAGAIN
    if (!prvTilting())
      position.ti(!prvTilting())
      position.tiltvalue = nextPosition.tiltvalue;
    else
      position.tiltvalue = (lastPosition.panvalue <= nextPosition.panvalue)?
	lastPosition.panvalue + (now - timeLastSet).msec() * params_.tiltRadPerMSec :
	lastPosition.panvalue - (now - timeLastSet).msec() * params_.tiltRadPerMSec;
#endif

    return position;
  }
};


