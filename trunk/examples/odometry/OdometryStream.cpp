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


#include "OdometryStream.h"

#include "miro/Server.h"
#include "idl/MotionStatusC.h"
#include "miro/IO.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

OdometryStream::OdometryStream(EventChannel_ptr _ec,
			       const std::string& domainName) :
  Super(_ec, false)
{
  EventTypeSeq added(1);
  EventTypeSeq removed(1);
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup("Odometry");

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumerAdmin_->subscription_change(added, removed);

  connect();
}

OdometryStream::~OdometryStream()
{
}

void
OdometryStream::push_structured_event(const StructuredEvent & notification
				    ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  const Miro::MotionStatusIDL * pOdometry;

  if (notification.remainder_of_body >>= pOdometry) {
    cout << *pOdometry << endl;
  }
  else
    cerr << "No OdometryIDL message." << endl;
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
 
    // The consumer, that gets the events
    OdometryStream pushConsumer(ec.in(), 
				server.namingContextName);
    
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
