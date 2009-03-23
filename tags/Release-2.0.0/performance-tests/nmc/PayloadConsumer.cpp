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
