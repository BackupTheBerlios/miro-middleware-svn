// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ButtonsNotify.h"

#include "miro/Server.h"
#include "miro/ButtonsC.h"
#include "miro/IO.h"

using std::cout;
using std::cerr;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;


ButtonsNotify::ButtonsNotify(EventChannel_ptr _ec,
					   const std::string& domainName) :
  Super(_ec)
{
  EventTypeSeq added(1);
  EventTypeSeq removed(1);
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup("Button");

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumerAdmin_->subscription_change(added, removed);
}

ButtonsNotify::~ButtonsNotify()
{
}

void
ButtonsNotify::push_structured_event(const StructuredEvent & notification
					    ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  const Miro::ButtonStatusIDL * pButtonStatus;

  cout << "Domain:  " << notification.header.fixed_header.event_type.domain_name << "\t"
       << "Type:    " << notification.header.fixed_header.event_type.type_name << endl;

  if (notification.remainder_of_body >>= pButtonStatus)
    cout << "ButtonStatus - time:" << pButtonStatus->time
	 << " name:" << pButtonStatus->number 
	 << " event:" << pButtonStatus->event << endl;
  else
    cerr << "No ButtonStatusIDL message." << endl;
}

int
main(int argc, char *argv[])
{
  cout << "Initialize server daemon." << endl;
  Miro::Server server(argc, argv);

  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));
    // The consumer, that gets the events
    ButtonsNotify pushConsumer(ec.in(), server.namingContextName);
    
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
