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
#include "TransitionMonitor.h"

#include "miro/Server.h"
#include "miro/Log.h"

TransitionMonitor::TransitionMonitor(CosNotifyChannelAdmin::EventChannel_ptr _ec,
				     const std::string& domainName) :
  Super(_ec)
{
  CosNotification::EventTypeSeq added;
  added.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup("PatternChange");

  setSubscriptions(added);
}

TransitionMonitor::~TransitionMonitor()
{
}

void
TransitionMonitor::push_structured_event(const CosNotification::StructuredEvent & notification
					 ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  char const * transition;
  if (notification.remainder_of_body >>= transition) {
    std::cout << "Transition: " << transition << std::endl;
  }
  else
    MIRO_LOG_OSTR(LL_ERROR, 
		  "TransitionMonitor: Unexpected event payload:" <<
		  notification.remainder_of_body.type()->id());

  std::cout << "Behaviour: " 
	    <<  notification.header.fixed_header.event_name << std::endl;

}

int
main(int argc, char *argv[])
{
  int rc = 1;
  Miro::Log::init(argc, argv);
  Miro::Server server(argc, argv);

  try {
    // The one channel that we create using the factory.
    CosNotifyChannelAdmin::EventChannel_var ec = 
      server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel");

    // The consumer, that gets the events
    TransitionMonitor pushConsumer(ec.in(), 
				   server.namingContextName);
    
    MIRO_LOG(LL_NOTICE, "Loop forever handling events.");
    server.run();
    MIRO_LOG(LL_NOTICE, "Server stoped, exiting.");

    pushConsumer.disconnect();
    rc = 0;
  }
  catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Uncaught CORBA exception: " << e);
  }
  return 0;
}
