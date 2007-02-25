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
#include "GenericNotify.h"

#include "miro/Server.h"
#include "miro/Log.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

GenericNotify::GenericNotify(EventChannel_ptr _consumerAdmin,
			     const char * domainName, const char * eventType) :
  Super(_consumerAdmin)
{
  EventTypeSeq added;
  added.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName);
  added[0].type_name = CORBA::string_dup(eventType);

  setSubscriptions(added);
}

GenericNotify::~GenericNotify()
{
}

ACE_Time_Value t(0);
int counter = 0;


void
GenericNotify::push_structured_event(const StructuredEvent & notification
				   ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  if (!counter)
    t = ACE_OS::gettimeofday();

  if (!(++counter % 50)) {
    ACE_Time_Value next = ACE_OS::gettimeofday();
    cout << "Events per second: " << 50. / ((next - t).usec() / 1000.) << endl;
    t = next;
  }

  cout << "Domain:  " << notification.header.fixed_header.event_type.domain_name << "\t"
       << "Type:    " << notification.header.fixed_header.event_type.type_name << endl;
}

int
main(int argc, char *argv[])
{
  Miro::Log::init(argc, argv);
  cout << "Initialize server daemon." << flush;
  Miro::Server server(argc, argv);

  if (argc < 2) {
    cout << "usage: " << argv[0] << " <TypeName> [DomainName]" << endl;
    return 1;
  }
    
  std::string domainName = server.namingContextName;
  if (argc > 2)
    domainName = argv[2];
  
  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));
    
    // The consumer, that gets the events
    GenericNotify pushConsumer(ec.in(), 
			       domainName.c_str(),
			       argv[1]);
    
    cout << "Loop forever handling events." << endl;
    server.run();
    cout << "Server stoped, exiting." << endl;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
