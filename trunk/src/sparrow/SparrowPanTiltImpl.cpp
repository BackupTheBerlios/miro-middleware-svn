// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "SparrowPanTiltImpl.h"
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
  PanTiltImpl::PanTiltImpl(BaseConnection * _connection,
			   Miro::StructuredPushSupplier * _pSupplier) :
    Miro::PanImpl(Parameters::instance()->pan),
    connection_(dynamic_cast<Connection2003 *>(_connection)),
    params_(Parameters::instance()->pan),
    pSupplier_(_pSupplier),
    lastPosition(Miro::deg2Rad(90.)),
    nextPosition(0.),
    timeLastSet(ACE_OS::gettimeofday()),
    totalLatency(params_.panLatency + params_.panSwing)
  {
    connection_->setPan(Miro::deg2Rad(0.));

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
  PanTiltImpl::setPan(CORBA::Float _value) throw (EDevIO, EOutOfBounds)
  {
    if (!params_.servo && isnan(connection_->getPanPosition()))
      return;

    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);

    if (_value != nextPosition) {

      // get current time
      ACE_Time_Value t = ACE_OS::gettimeofday();

      // set servo
      connection_->setPan(_value);

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
  
  void
  PanTiltImpl::setPanExt(CORBA::Float _value, CORBA::Float _speed ) throw (EDevIO, EOutOfBounds)
  {
    if (!params_.servo && isnan(connection_->getPanPosition()))
      return;

    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);

    if (_value != nextPosition) {

      // get current time
      ACE_Time_Value t = ACE_OS::gettimeofday();

      // set servo
      connection_->setPanExt(_value, _speed);

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


  void
  PanTiltImpl::setPanSpeed(CORBA::Float _speed) throw (EDevIO, EOutOfBounds)
  {
     connection_->setPanExt(nextPosition, _speed);
  }
  
  CORBA::Float
  PanTiltImpl::getPan() throw (EDevIO)
  {
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);
    ACE_Time_Value t = ACE_OS::gettimeofday();
    return currentPosition(t).angle;
  }

  CORBA::Boolean
  PanTiltImpl::panning(const Miro::TimeIDL& stamp) throw()
  {
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);
    if (!params_.servo) {
      float pan = connection_->getPanPosition();
      return fabs(targetPan_ - pan) > Miro::deg2Rad(2);
    }
    ACE_Time_Value t;
    Miro::timeC2A(stamp, t);
    return prvPanning(t);
  }

  Miro::PanPositionIDL
  PanTiltImpl::currentPan(const Miro::TimeIDL& stamp) throw()
  {
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);
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
      position.angle = connection_->getPanPosition();
      position.accuracy = params_.panAccuracy;
    }

    return position;
  }
}


