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

#include "GenericNotify.h"

#include "miro/Server.h"

using std::cout;
using std::cerr;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

GenericNotify::GenericNotify(EventChannel_ptr _consumerAdmin,
			     const char * domainName, const char * eventType) :
  Super(_consumerAdmin)
{
  EventTypeSeq added(1);
  EventTypeSeq removed(1);
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName);
  added[0].type_name = CORBA::string_dup(eventType);

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumerAdmin_->subscription_change(added, removed);
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
  cout << "Initialize server daemon." << flush;
  Miro::Server server(argc, argv);

  if (argc < 2) {
    cout << "usage: " << argv[0] << " <TypeName>" << endl;
    return 1;
  }
    
  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));
    
    // The consumer, that gets the events
    GenericNotify pushConsumer(ec.in(), 
			       server.namingContextName.c_str(),
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
