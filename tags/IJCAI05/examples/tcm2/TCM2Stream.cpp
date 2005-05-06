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

#include "TCM2Stream.h"

#include "miro/Server.h"
#include "miro/IO.h"
#include "idl/TCM2EventC.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

TCM2Stream::TCM2Stream(EventChannel_ptr _ec,
		       const std::string& domainName) :
  Super(_ec)
{
  EventTypeSeq added;
  added.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup("TCM2");

  setSubscriptions(added);
}

TCM2Stream::~TCM2Stream()
{
}

void
TCM2Stream::push_structured_event(const StructuredEvent & notification
				    ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  const Miro::TCM2EventIDL * pTCM2Data;

  if (notification.remainder_of_body >>= pTCM2Data)
    cout << *pTCM2Data << endl;
  else
    cerr << "No TCM2EventIDL in message." << endl;
}

int
main(int argc, char *argv[])
{
  Miro::Server server(argc, argv);

  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));

    cerr << "press return to start..." << flush;
    getchar();

    // The consumer that gets the events
    TCM2Stream pushConsumer(ec.in(), server.namingContextName);

    cerr << "Loop forever handling events." << endl;
    server.run();
    cerr << "Server stoped, exiting." << endl;

    pushConsumer.disconnect();
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
