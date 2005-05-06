// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
