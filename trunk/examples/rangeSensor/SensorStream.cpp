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
#include "miro/Log.h"

#include "idl/RangeEventC.h"

#include <map>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using CosNotification::EventTypeSeq;
using CosNotification::StructuredEvent;
using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::EventChannel_var;

int argCounter;
char ** argVector;
unsigned short group;
bool group_en;

SensorStream::SensorStream(EventChannel_ptr _ec,
			 const std::string& domainName,
			 const std::string& sensorName) :
  Super(_ec, false)
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
  connect();
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

    if(group_en && group == pSensorGroup->group){
    for( int i = 2; i < argCounter-2; i++ ) {
      unsigned int k;
      sscanf( argVector[ i ], " %d ", &k );

      for (unsigned int j = 0; j < pSensorGroup->range.length(); ++j)
	if (j == k) {
          cout.width(6);

	  cout << pSensorGroup->range[j] << "\t";
	}
    }
    cout << endl;
    }
    if(!group_en){
    for( int i = 2; i < argCounter; i++ ) {
      unsigned int k;
      sscanf( argVector[ i ], " %d ", &k );

      for (unsigned int j = 0; j < pSensorGroup->range.length(); ++j)
	if (j == k) {
          cout.width(6);
	  cout << pSensorGroup->range[j] << "\t";
	}



    }
    cout << endl;
    }
  }
  else
    MIRO_LOG_OSTR("SensorStream",LM_ERROR, "No SensorIDL message." << endl);
}

int main(int argc, char *argv[])
{
  Miro::Server server(argc, argv);

  if( argc <= 2 ) {
    cout << "usage: " << argv[0] << " <sensor name> <sensor #1> <sensor #2> ...<-group group#>" << endl;
    cout << "prints sensor values of the specified sensor." << endl;

    return 0;
  }
  group = 0;
  group_en = false;

  if(argc >=4 && (strcmp(argv[argc-2], "-group") == 0)){
     group = atoi(argv[argc-1]);
     group_en = true;

     MIRO_LOG_OSTR("SensorStream",LM_INFO, "Group: " << group << endl);

  }

  argCounter = argc;
  argVector = argv;

  
  try {
    // The one channel that we create using the factory.
    EventChannel_var ec(server.resolveName<EventChannel>("EventChannel"));

    cout << "press return to start..." << flush;

    getchar();

    // The consumer, that gets the events
    SensorStream pushConsumer(ec.in(),
			      server.namingContextName,
			      std::string(argv[1]) );

    MIRO_LOG_OSTR("SensorStream",LM_INFO, "Loop forever handling events." << endl);
    server.run();
    MIRO_LOG_OSTR("SensorStream",LM_INFO, "Server stopped, exiting." << endl);
  }
  catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR("SensorStream",LM_CRITICAL, "Uncaught CORBA exception: " << e << endl);
    return 1;
  }
  catch (...) {
    MIRO_LOG_OSTR("SensorStream",LM_CRITICAL, "Uncaught exception!" << endl);
    return 1;
  }
  return 0;
}
