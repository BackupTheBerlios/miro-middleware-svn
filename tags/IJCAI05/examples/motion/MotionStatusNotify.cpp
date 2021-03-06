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
