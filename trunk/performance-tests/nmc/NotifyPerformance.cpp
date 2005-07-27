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
#include "idl/MotionStatusC.h"
#include "idl/RangeEventC.h"

#include "PayloadConsumer.h"

#include "miro/Server.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"
#include "miro/TimeHelper.h"
#include "miro/NotifyMulticast.h"
#include "miro/Configuration.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

#include <orbsvcs/Time_Utilities.h>

#include <ace/Get_Opt.h>
#include <ace/Stats.h>
#include <ace/Sample_History.h>
#include <ace/High_Res_Timer.h>
#include <ace/Sched_Params.h>

#include <iostream>
#include <vector>

enum PayloadID {
  NONE, 
  OCTED_STREAM_1K, OCTET_STREAM_100K,
  INT_ARRAY_1K, INT_ARRAY_10K,
  MOTION_STATUS, RANGE_EVENT,
  SHARED_BELIEF, SHARED_BELIEF_FULL
};


const unsigned int NUM_PAYLOADS = 9;
char const * const payloadName[NUM_PAYLOADS] = {
  "None",
  "OctetStream1K",
  "OctetStream100K",
  "IntArray1K",
  "IntArray10K",
  "MotionStatus",
  "RangeSensor",
  "SharedBelief",
  "SharedBeliefFull"
};

bool verbose = false;
PayloadID payload = OCTED_STREAM_1K;
int iterations = 10000;
int colocated = 0;
bool multicast = false;
ACE_Time_Value timeout = 0;



void 
producePayload(CosNotifyChannelAdmin::EventChannel_ptr _ec, std::string const& _nc)
{
  CosNotification::EventTypeSeq offers;
  offers.length(1);
  offers[0].domain_name = CORBA::string_dup(_nc.c_str());
  offers[0].type_name = CORBA::string_dup("Payload");
  Miro::StructuredPushSupplier supplier(_ec,
					_nc,
					true,
					offers);
  CosNotification::StructuredEvent event;
  Miro::StructuredPushSupplier::initStructuredEvent(event, _nc, "Payload");

  ACE_Sample_History history (iterations);
  // ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  // for (int i = 0; i < iterations; ++i) {

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
  case INT_ARRAY_10K: 
    {
      I10K * load = new I10K();
      for (unsigned int i = 0; i < 2560; ++i)
	load->array[i] = i;
      event.remainder_of_body <<= load;
      break;
    }
  case MOTION_STATUS:
    {
      Miro::MotionStatusIDL * load = new Miro::MotionStatusIDL();;
      Miro::timeA2C(ACE_OS::gettimeofday(), load->time);
      event.remainder_of_body <<= load;
      break;
    }
  case RANGE_EVENT:
    {
      Miro::RangeScanEventIDL * load = new Miro::RangeScanEventIDL();
      Miro::timeA2C(ACE_OS::gettimeofday(), load->time);
      load->range.length(1);
      load->range[0].length(361);
      event.remainder_of_body <<= load;
      break;
    }
  case SHARED_BELIEF: 
    {
      MSL::SharedBeliefState01 * load = new MSL::SharedBeliefState01();
      event.remainder_of_body <<= load;
      break;
    }
  case SHARED_BELIEF_FULL: 
    {
      MSL::SharedBeliefState01 * load = new MSL::SharedBeliefState01();

      // set the time stamp
      ORBSVCS_Time::Time_Value_to_TimeT(load->time, ACE_OS::gettimeofday());

      load->egoBelief.oppGoal.length(2);
      load->egoBelief.ownGoal.length(2);
      load->egoBelief.observation.length(15);
      for (unsigned int i = 0; i < 15; ++i) {
	load->egoBelief.observation[i].classification.length(2);
      }
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

  

  // send two dummies, to get the initializing done...
 
  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < iterations; ++i) {
    ACE_Time_Value then = ACE_OS::gettimeofday() + timeout;
    ACE_hrtime_t start = ACE_OS::gethrtime ();
    supplier.sendEvent(event);
    ACE_hrtime_t end = ACE_OS::gethrtime ();
    history.sample (end - start);
    ACE_Time_Value now = ACE_OS::gettimeofday();
    if (now < then)
      ACE_OS::sleep(then - now);
  }

  ACE_hrtime_t test_end = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_DEBUG, "test finished\n"));
  
  ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));
  
  std::cerr << "Scale facor: " << gsf << std::endl;
  //  history.dump_samples ("HISTORY", gsf);
  
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
  ACE_Get_Opt get_opts (argc, argv, "t:c:mn:p:v?");
  
  int rc = 0;
  int c;
  
  while ((c = get_opts()) != -1) {
    switch (c) {
    case 't':
      timeout = ACE_Time_Value(0, atoi(get_opts.optarg) * 1000);
    case 'm':
      multicast = true;
      break;
    case 'c':
      colocated = atoi(get_opts.optarg);
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
		<< "  -m enamble multicast" << std::endl
		<< "  -c <number> colocated consumers (default 0)" << std::endl
		<< "  -n <iterations> number of iterations" << std::endl
		<< "  -t <msec> timeout ( < 1000, default 0)" << std::endl
		<< "  -p <payload> the type of payload for the test:" << std::endl;
      for (unsigned int i = 0; i < NUM_PAYLOADS; ++i) {
	std::cerr << "      " << payloadName[i] << std::endl;
      }
      std::cerr << "  -v verbose mode" << std::endl
		<< "  -? help: emit this text and stop" << std::endl;
      rc = -1;
    }
  }

  if (verbose) {
    std::cout << "multicast: " << multicast << std::endl
	      << "colocated clients: " << colocated << std::endl
	      << "iterations: " << iterations << std::endl
	      << "payload: " << payloadName[payload] << std::endl;
  }
  return rc;
}



int
main (int argc, char * argv[])
{
  int rc = 1;

  Miro::Log::init(argc, argv);
  Miro::Configuration::init(argc, argv);

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParams = Miro::RobotParameters::instance();
  Miro::NMC::Parameters * nmcParams = Miro::NMC::Parameters::instance();
    
  // Config file processing
  Miro::ConfigDocument * config =  Miro::Configuration::document();
  config->setSection("Robot");
  config->getParameters("Miro::RobotParameters", *robotParams);

  config->setSection("Notification");
  config->getParameters("Miro::NMC::Parameters", *nmcParams);
  config->fini();

  try {
    Miro::Server server(argc, argv);

    if (parseArgs(argc, argv) != 0)
      return 1;
    
    TAO_Notify_Default_CO_Factory::init_svc();
    TAO_Notify_Default_POA_Factory::init_svc();
    TAO_Notify_Default_Collection_Factory::init_svc();
    TAO_Notify_Default_EMO_Factory::init_svc();
    
    try {

      ACE_Sched_Params schedp(ACE_SCHED_RR, 10, ACE_SCOPE_PROCESS);
      if (ACE_OS::sched_params(schedp) == -1)
	std::cout << "Warning: Could not set rt scheduler." << std::endl;

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
      // register at the naming service
      server.addToNameService(ec.in(), "EventChannel");

      // initialize notify multicast
      Miro::NMC::Adapter *mcAdapter = NULL;
      if (multicast) {
	nmcParams->subscription.insert("Payload");
	mcAdapter =
	  new Miro::NMC::Adapter(argc, argv, 
				 &server, ec.in(),
				 server.namingContextName);
	mcAdapter->init();
      }

      // create local consumers.
      std::vector<PayloadConsumer *> consumer;
      for (int i = 0; i < colocated; ++i)
	consumer.push_back(new PayloadConsumer(ec.in(), server.namingContextName, i));

      std::cout << "press return to start..." << std::flush;
      getchar();
      std::cout << std::endl;
      ACE_OS::sleep(1);
      
      producePayload(ec, server.namingContextName);

      for (int i = 0; i < colocated; ++i) {
	consumer[i]->results();
	delete consumer[i];
      }

      // clean up multicast module
      if (mcAdapter) {
	mcAdapter->fini();
	delete mcAdapter;
      }

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

