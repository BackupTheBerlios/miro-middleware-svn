// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PayloadConsumer.h"

#include "miro/Server.h"

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
				 unsigned int _id) :
  Super(_consumerAdmin),
  id_(_id),
  received_(0)
{
  EventTypeSeq added;
  added.length(1);

  added[0].domain_name =  CORBA::string_dup("Miro");
  added[0].type_name = CORBA::string_dup("Payload");

  setSubscriptions(added);
}

PayloadConsumer::~PayloadConsumer()
{
}

void
PayloadConsumer::push_structured_event(const StructuredEvent & /*notification*/)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  ++received_;
}

void
PayloadConsumer::results()
{
  std::cout << "PayloadConsumer " << id_ << " - Events received: " << received_ << std::endl;
}
