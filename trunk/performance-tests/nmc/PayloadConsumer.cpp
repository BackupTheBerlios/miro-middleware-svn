// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PayloadConsumer.h"
#include "SharedBeliefStateC.h"

#include "idl/MotionStatusC.h"
#include "miro/Server.h"
#include "miro/TimeHelper.h"

#include <orbsvcs/Time_Utilities.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

PayloadConsumer::PayloadConsumer(EventChannel_ptr _consumerAdmin,
				 std::string const& _nc,
				 unsigned int _id) :
  Super(_consumerAdmin),
  id_(_id),
  received_(0)
{
  EventTypeSeq added;
  added.length(1);

  added[0].domain_name =  CORBA::string_dup(_nc.c_str());
  added[0].type_name = CORBA::string_dup("Payload");

  setSubscriptions(added);
}

PayloadConsumer::~PayloadConsumer()
{
}

void
PayloadConsumer::push_structured_event(const StructuredEvent & notification)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  ++received_;
  MSL::SharedBeliefState01 * belief = new MSL::SharedBeliefState01();
  Miro::MotionStatusIDL * motion = new Miro::MotionStatusIDL();;

  if (notification.remainder_of_body >>= belief) {
    ACE_Time_Value now = ACE_OS::gettimeofday();
    ACE_Time_Value then;
    ORBSVCS_Time::TimeT_to_Time_Value(now, belief->time);
    std::cout << then << " " << now << " " << (now - then) << std::endl;
  }
  else if (notification.remainder_of_body >>= motion) {
    ACE_Time_Value now = ACE_OS::gettimeofday();
    ACE_Time_Value then;
    Miro::timeC2A(motion->time, then);
    std::cout << then << " " << now << " " << (now - then) << std::endl;
  }
}

void
PayloadConsumer::results()
{
  std::cout << "PayloadConsumer " << id_ << " - Events received: " << received_ << std::endl;
}
