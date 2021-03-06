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


#include "SensorStream.h"

#include "miro/Server.h"
#include "miro/RangeEventC.h"

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

SensorStream::SensorStream(EventChannel_ptr _ec,
			 const std::string& domainName,
			 const std::string& sensorName) :
  Super(_ec)
{
  EventTypeSeq added(1);
  EventTypeSeq removed(1);
  added.length(1);
  removed.length(1);

  added[0].domain_name =  CORBA::string_dup(domainName.c_str());
  added[0].type_name = CORBA::string_dup(sensorName.c_str());

  removed[0].domain_name =  CORBA::string_dup("*");
  removed[0].type_name = CORBA::string_dup("*");

  consumerAdmin_->subscription_change(added, removed);
}

SensorStream::~SensorStream()
{
}

void
SensorStream::push_structured_event(const StructuredEvent & notification
				    ACE_ENV_ARG_DECL_NOT_USED)
  throw(CORBA::SystemException, CosEventComm::Disconnected)
{
  const Miro::RangeBunchEventIDL * pSensorBunch;
  const Miro::RangeGroupEventIDL * pSensorGroup;

  if (notification.remainder_of_body >>= pSensorBunch) {

    for( int i = 2; i < argCounter; i++ ) {
      int k;
      sscanf( argVector[ i ], " %i ", &k );

      for (unsigned int j = 0; j < pSensorBunch->sensor.length(); ++j)
	if (pSensorBunch->sensor[j].index == k) {
          cout.width(6);
	  cout << pSensorBunch->sensor[j].range << "\t";
	}
    }
    cout << endl;
  }
  else if (notification.remainder_of_body >>= pSensorGroup) {

    for( int i = 2; i < argCounter; i++ ) {
      unsigned int k;
      sscanf( argVector[ i ], " %i ", &k );

      for (unsigned int j = 0; j < pSensorGroup->range.length(); ++j)
	if (j == k) {
          cout.width(6);
	  cout << pSensorGroup->range[j] << "\t";
	}
    }
    cout << endl;
  }
  else
    cerr << "No SensorIDL message." << endl;
}

int
main(int argc, char *argv[])
{
  Miro::Server server(argc, argv);

  if( argc <= 2 ) {
    cout << "usage: " << argv[0] << " <sensor name> <sensor #1> <sensor #2> ..." << endl;
    cout << "prints sensor values of the specified sensor." << endl;
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
    SensorStream pushConsumer(ec.in(), 
			      server.namingContextName, 
			      std::string(argv[1]) );
    
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
