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


#include "SonarStream2.h"

#include "miro/Server.h"
#include "idl/RangeEventC.h"

#include <map>

using std::cout;
using std::cerr;
using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

int argCounter;
char ** argVector;

SonarStream::SonarStream(EventChannel_ptr _ec,
			 const std::string& domainName) :
  Super(_ec)
{
  EventTypeSeq added(1);
  EventTypeSeq removed(1);
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup("Sonar");

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumerAdmin_->subscription_change(added, removed);
}

SonarStream::~SonarStream()
{
}

void
SonarStream::push_structured_event(const StructuredEvent & notification
				   ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  const Miro::RangeBunchEventIDL * pSonarEvent;

  if (notification.remainder_of_body >>= pSonarEvent) {

    for( int i = 1; i < argCounter; i++ ) {
      int k;
      sscanf( argVector[ i ], " %i ", &k );

      for (unsigned int j = 0; j < pSonarEvent->sensor.length(); ++j)
	if (pSonarEvent->sensor[j].index == k) {
	  cout << pSonarEvent->sensor[j].range << "\t" << endl;
	}
    }
  }
  else
    cerr << "No SonarEventIDL message." << endl;
}

int
main(int argc, char *argv[])
{
  Miro::Server server(argc, argv);

  if( argc <= 1 ) {
    cout << "usage: <sonar#1> <sonar#2> ..." << endl;
    cout << "prints sonarvalues of the specified sonars." << endl;
    return 0;
  }

  argCounter = argc;
  argVector = argv;
    
  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));

    cerr << "press return to start..." << flush;
    getchar();
 
    // The consumer, that gets the events
    SonarStream pushConsumer(ec.in(), server.namingContextName);
    
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
