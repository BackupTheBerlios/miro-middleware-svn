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

#include "PayloadC.h"
#include "SharedBeliefStateC.h"

#include "miro/Server.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/LogNotifyConsumer.h"
#include "miro/Log.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

#include "ace/Get_Opt.h"
#include <ace/Stats.h>
#include <ace/Sample_History.h>
#include <ace/High_Res_Timer.h>


#include <iostream>

enum PayloadID {
  NONE, 
  OCTED_STREAM_1K, OCTET_STREAM_100K,
  INT_ARRAY_1K, INT_ARRAY_100K,
  SHARED_BELIEF 
};


const unsigned int NUM_PAYLOADS = 6;
char const * const payloadName[NUM_PAYLOADS] = {
  "None",
  "OctedStream1K",
  "OctedStream100K",
  "IntArray1K",
  "IntArray10K",
  "SharedBeliefState01"
};

bool verbose = false;
PayloadID payload = OCTED_STREAM_1K;
std::string fileName;
int iterations = 10000;


void 
producePayload(CosNotifyChannelAdmin::EventChannel_ptr _ec)
{
  CosNotification::EventTypeSeq offers;
  offers.length(1);
  offers[0].domain_name = CORBA::string_dup("Miro");
  offers[0].type_name = CORBA::string_dup("Log");
  Miro::StructuredPushSupplier supplier(_ec,
					"Miro",
					true,
					offers);
  CosNotification::StructuredEvent event;
  Miro::StructuredPushSupplier::initStructuredEvent(event, "Miro", "Log");

  switch(payload) {
  case NONE: 
    {
      break;
    }
  case OCTED_STREAM_1K: 
    {
      OctStr_var load = new OctStr;
      load->length(1024);
      for (unsigned int i = 0; i < load->length(); ++i)
	load[i] = (CORBA::Octet) i;
      event.remainder_of_body <<= load._retn();
      break;
    }
  case OCTET_STREAM_100K: 
    {
      OctStr_var load = new OctStr;
      load->length(1024 * 100);
      for (unsigned int i = 0; i < load->length(); ++i)
	load[i] = (CORBA::Octet) i;
      event.remainder_of_body <<= load._retn();
      break;
    }
  case INT_ARRAY_1K: 
    {
      I1K * load = new I1K();
      for (unsigned int i = 0; i < 256; ++i)
	load->array[i] = i;
      event.remainder_of_body <<= load;
      break;
    }
  case INT_ARRAY_100K: 
    {
      I10K * load = new I10K();
      for (unsigned int i = 0; i < 2560; ++i)
	load->array[i] = i;
      event.remainder_of_body <<= load;
      break;
    }
  case SHARED_BELIEF: 
    {
      MSL::SharedBeliefState01 * load = new MSL::SharedBeliefState01();
      event.remainder_of_body <<= load;
      break;
    }
  default: 
    {
      std::ostringstream ostr;
      ostr << "Unknown payload: " << payload;
      throw Miro::Exception(ostr.str());
    }
  }

  ACE_Sample_History history (iterations);
  

  // send two dummies, to get the initializing done...
 
  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < iterations; ++i) {
    ACE_hrtime_t start = ACE_OS::gethrtime ();
    supplier.sendEvent(event);
    ACE_hrtime_t now = ACE_OS::gethrtime ();
    history.sample (now - start);
  }

  ACE_hrtime_t test_end = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_DEBUG, "test finished\n"));
  
  ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));
  
  history.dump_samples ("HISTORY", gsf);
  
  ACE_Basic_Stats stats;
  history.collect_basic_stats (stats);
  stats.dump_results ("Total", gsf);
  
  ACE_Throughput_Stats::dump_throughput ("Total", gsf,
					 test_end - test_start,
					 stats.samples_count ());
}

int 
parseArgs(int& argc, char* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:f:p:v?");
  
  int rc = 0;
  int c;
  
  while ((c = get_opts()) != -1) {
    switch (c) {
    case 'f':
      fileName = get_opts.optarg;
      break;
    case 'n':
      iterations = atoi(get_opts.optarg);
      break;
    case 'p':
      for (unsigned int i = 0; i < NUM_PAYLOADS; ++i) {
	if (strcmp(payloadName[i], get_opts.optarg) == 0) {
	  payload = (PayloadID) i;
	  break;
	}
      }
      break;
    case 'v':
      verbose = true;
      break;
    case '?':
    default:
      std::cerr << "usage: " << argv[0] << "[-fpv?]" << std::endl
		<< "  -n <iterations> number of iterations" << std::endl
		<< "  -f <file name> log file name" << std::endl
		<< "  -p <payload> the type of payload for the test" << std::endl
		<< "  -v verbose mode" << std::endl
		<< "  -? help: emit this text and stop" << std::endl;
      rc = -1;
    }
  }

  if (verbose) {
    std::cout << "iterations: " << iterations << std::endl
	      << "log file name: " << fileName << std::endl
	      << "payload: " << payloadName[payload] << std::endl;
  }
  return rc;
}



int
main (int argc, char * argv[])
{
  int rc = 1;

  try {
    Miro::Log::init(argc, argv);
    Miro::Server server(argc, argv);

    if (parseArgs(argc, argv) != 0)
      return 1;
    
    TAO_Notify_Default_CO_Factory::init_svc();
    TAO_Notify_Default_POA_Factory::init_svc();
    TAO_Notify_Default_Collection_Factory::init_svc();
    TAO_Notify_Default_EMO_Factory::init_svc();
    
    try {

      // start server thread in the background
      server.detach(1);

      // build an event channel

      // Notification Channel Factory
      CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory =
	TAO_Notify_EventChannelFactory_i::create(server.poa.in() ACE_ENV_ARG_PARAMETER);
      // Initial qos specified to the factory when creating the EC.
      CosNotification::QoSProperties initialQos;
      // Initial admin props specified to the factory when creating the EC.
      CosNotification::AdminProperties initialAdmin;
      // Channel identifer
      CosNotifyChannelAdmin::ChannelID id;
      // The one channel that we create using the factory
      CosNotifyChannelAdmin::EventChannel_var ec =
	notifyFactory->create_channel(initialQos, initialAdmin, id);

      Miro::LogNotifyParameters params;
      params.event.resize(1);
      params.event[0].domain = "Miro";
      params.event[0].type = "Log";
      Miro::LogNotifyConsumer consumer(server, ec, server.namingContextName, fileName, params);
      
      producePayload(ec);

      server.shutdown();
      server.wait();
      rc = 0;
    }
    catch(CORBA::Exception const& e) {
      std::cerr << "Uncought CORBA exception:\n" << e << std::endl;
    }
   
  }
  catch (Miro::Exception const& e)
  {
    std::cerr << "Uncought Miro exception:\n" << e << std::endl;
  }
  return rc;
}

