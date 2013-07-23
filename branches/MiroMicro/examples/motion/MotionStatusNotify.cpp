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
#include "MotionStatusNotify.h"

#include "miro/Server.h"
#include "miro/IO.h"
#include "idl/MotionStatusC.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

MotionStatusNotify::MotionStatusNotify(EventChannel_ptr _ec,
				       const std::string& domainName) :
  Super(_ec)
{
  EventTypeSeq added;
  added.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup("Odometry");

  setSubscriptions(added);
}

MotionStatusNotify::~MotionStatusNotify()
{
}

void
MotionStatusNotify::push_structured_event(const StructuredEvent & notification
					  ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  const Miro::MotionStatusIDL * pMotionStatus;

  cout << "Domain:  " << notification.header.fixed_header.event_type.domain_name << endl
       << "Type:    " << notification.header.fixed_header.event_type.type_name << endl;

  if (notification.remainder_of_body >>= pMotionStatus)
    cout << *pMotionStatus << endl;
  else
    cerr << "No MotionStatusIDL message." << endl;
}

int
main(int argc, char *argv[])
{
  cout << "Initialize server daemon." << flush;
  Miro::Server server(argc, argv);

  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));
    // The consumer, that gets the events
    MotionStatusNotify pushConsumer(ec.in(), server.namingContextName);
    
    cout << "Loop forever handling events." << endl;
    server.run();
    cout << "Server stoped, exiting." << endl;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception!" << endl;
    return 1;
  }
  return 0;
}
