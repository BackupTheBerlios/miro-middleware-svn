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


#include "SparrowAlive.h"

#include "miro/Server.h"
#include "idl/SparrowAliveC.h"

#include <map>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;


SparrowAlive::SparrowAlive(EventChannel_ptr _ec,
			 const std::string& domainName) :
  Super(_ec)
{
  EventTypeSeq added;
  added.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup("SparrowAlive");

  setSubscriptions(added);
}

SparrowAlive::~SparrowAlive()
{
}

void
SparrowAlive::push_structured_event(const StructuredEvent & notification
				    ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  const Miro::SparrowAliveIDL * pSparrowAlive;

  if (notification.remainder_of_body >>= pSparrowAlive) {



     cout << " " << endl;
     cout << "MotorAlive: " << (bool) pSparrowAlive->MotorAlive << endl;
     cout << "Infrared1Alive: " << (bool) pSparrowAlive->Infrared1Alive << endl;
     cout << "Infrared2Alive: " << (bool) pSparrowAlive->Infrared2Alive << endl;
     cout << "PanAlive: " << (bool) pSparrowAlive->PanAlive << endl;
     cout << "KickerAlive: " << (bool) pSparrowAlive->KickerAlive << endl;


     cout << " " << endl;

  }

  else
    cerr << "No SparrowAliveIDL message." << endl;
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
    SparrowAlive pushConsumer(ec.in(),
			      server.namingContextName);

    cerr << "Loop forever handling events." << endl;
    server.run();
    cerr << "Server stoped, exiting." << endl;
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
