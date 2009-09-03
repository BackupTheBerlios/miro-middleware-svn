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
#include "OdometryTracking.h"
#include "TimeHelper.h"
#include <miro/Log.h>

namespace Miro
{
  /**
   * @param _ec The event channel at which the consumer will
   * subscribe itself for the events.
   *
   * @param _domainName The domain name of the events. This is the
   * same as the name of the naming context, within which the robot
   * registers its services.
   */
  OdometryTracking::OdometryTracking(CosNotifyChannelAdmin::EventChannel_ptr _ec,
				     const std::string& _domainName) :
    Super(_ec),
    odoTruncate_(false),
    rawTruncate_(false)
  {
    MIRO_DBG(MIRO,LL_CTOR_DTOR, "Constructing OdometryTracking.\n");
    
    // subscribe for the events we'd like to get
    MIRO_DBG(MIRO,LL_NOTICE, "subscribe for events\n");
    CosNotification::EventTypeSeq added;
    added.length(2);

    added[0].domain_name = CORBA::string_dup(_domainName.c_str());
    added[0].type_name   = CORBA::string_dup("Odometry");
    added[1].domain_name = CORBA::string_dup(_domainName.c_str());
    added[1].type_name   = CORBA::string_dup("RawPosition");
    setSubscriptions(added);

    MIRO_DBG(MIRO,LL_NOTICE, "finished\n");
  }

  /**
   * Consumer for the Odometry and RawPosition events.
   * The incomeing events become stored in a bounded queue.
   */
  void
  OdometryTracking::push_structured_event(const CosNotification::StructuredEvent & notification
					  ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosEventComm::Disconnected)
  {
    MotionStatusIDL * odometry;
    RawPositionIDL * raw;

    if (::operator >>= (notification.remainder_of_body, odometry)) {
      Guard guard(odoMutex);
      if (!odoTruncate_ && odoDeque.size() > maxDequeSize)
	odoTruncate_ = true;
      if (odoTruncate_)
	odoDeque.pop_back();
      odoDeque.push_front(*odometry);
    }
    else if (notification.remainder_of_body >>= raw) {
      Guard guard(rawMutex);
      if (!rawTruncate_ && rawDeque.size() > maxDequeSize)
	rawTruncate_ = true;
      if (rawTruncate_)
	rawDeque.pop_back();
      rawDeque.push_front(*raw);
    }
    else {
      MIRO_LOG(LL_WARNING,"OdometryTracking: received message I did not subscribe for.\n");
    }
  }

  MotionStatusIDL
  OdometryTracking::getPosition(const ACE_Time_Value& stamp) const
  {
    Guard guard(odoMutex);
    ACE_Time_Value t;
    OdometryDeque::const_iterator first, last = odoDeque.end();
    for (first = odoDeque.begin(); first != last; ++first) {
      timeC2A(first->time, t);
      if (t <= stamp)
	break;
    }

    // TODO 
    // add interpolation to minimize the error on robots
    // with slow odometry updates

    //    ACE_Time_Value delta = stamp - t;
    //    double da  = (first->velocity.rotation / 1000.) * delta.msec();
    //    double dxy = (first->velocity.translation / 1000.) * delta.msec();


    return (first != last)? *first : MotionStatusIDL();
  }


  PositionIDL
  OdometryTracking::getRawPosition(const ACE_Time_Value& stamp) const
  {
    RawDeque::const_iterator first, last = rawDeque.end();
    for (first = rawDeque.begin(); first != last; ++first) {
      ACE_Time_Value t;
      timeC2A(first->time, t);
      if (t <= stamp)
	break;
    }

    // TODO 
    // add interpolation to minimize the error on robots
    // with slow odometry updates

    return (first != last)? first->position : PositionIDL();
  }
};