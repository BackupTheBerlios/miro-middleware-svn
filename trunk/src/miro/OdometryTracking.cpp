// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "OdometryTracking.h"
#include "TimeHelper.h"

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

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
    Super(_ec, false),
    odoTruncate_(false),
    rawTruncate_(false)
  {
    DBG(std::cout << "Constructing OdometryTracking." << std::endl);
    
    // subscribe for the events we'd like to get
    DBG(std::cout << "subscribe for events" << std::endl);
    CosNotification::EventTypeSeq added(2);
    CosNotification::EventTypeSeq removed(1);
    added.length(2);
    removed.length(1);

    added[0].domain_name = CORBA::string_dup(_domainName.c_str());
    added[0].type_name   = CORBA::string_dup("Odometry");
    added[1].domain_name = CORBA::string_dup(_domainName.c_str());
    added[1].type_name   = CORBA::string_dup("RawPosition");
    removed[0].domain_name =  CORBA::string_dup("*");
    removed[0].type_name = CORBA::string_dup("*");
    consumerAdmin_->subscription_change(added, removed);
    connect();

    DBG(std::cout << "finished" << std::endl);
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
      cerr << "OdometryTracking: received message I did not subscribe for." << endl;
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
