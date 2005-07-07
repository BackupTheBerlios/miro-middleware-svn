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

#include "PayloadConsumer.h"

#include "miro/Server.h"
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
#include <ace/Sched_Params.h>

#include <iostream>
#include <vector>

bool verbose = false;
int clients = 1;
bool multicast = false;
std::string domainName = "Miro";

int 
parseArgs(int& argc, char* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "d:c:mv?");
  
  int rc = 0;
  int c;
  
  while ((c = get_opts()) != -1) {
    switch (c) {
    case 'm':
      multicast = true;
      break;
    case 'c':
      clients = atoi(get_opts.optarg);
      break;
    case 'v':
      verbose = true;
      break;
    case 'd':
      domainName = get_opts.optarg;
      break;
    case '?':
    default:
      std::cerr << "usage: " << argv[0] << "[-fpv?]" << std::endl
		<< "  -m enamble multicast" << std::endl
		<< "  -c <number> client consumers (default 1)" << std::endl
		<< "  -d <name> domain name for the subscribed events (default Miro)" << std::endl
		<< "  -v verbose mode" << std::endl
		<< "  -? help: emit this text and stop" << std::endl;
      rc = -1;
    }
  }

  if (verbose) {
    std::cout << "multicast: " << multicast << std::endl
	      << "cients: " << clients << std::endl
	      << "domain name: " << domainName << std::endl;
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
    
    try {

      ACE_Sched_Params schedp(ACE_SCHED_RR, 10, ACE_SCOPE_PROCESS);
      if (ACE_OS::sched_params(schedp) == -1)
	std::cout << "Warning: Could not set rt scheduler." << std::endl;

      // start server thread in the background
      server.detach(1);

      // build an event channel

      // Notification Channel Factory
      CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory;
      // Initial qos specified to the factory when creating the EC.
      CosNotification::QoSProperties initialQos;
      // Initial admin props specified to the factory when creating the EC.
      CosNotification::AdminProperties initialAdmin;
      // Channel identifer
      CosNotifyChannelAdmin::ChannelID id;
      // The one channel that we create using the factory
      CosNotifyChannelAdmin::EventChannel_var ec;

      // initialize notify multicast
      Miro::NMC::Adapter *mcAdapter = NULL;
      if (multicast) {
	TAO_Notify_Default_CO_Factory::init_svc();
	TAO_Notify_Default_POA_Factory::init_svc();
	TAO_Notify_Default_Collection_Factory::init_svc();
	TAO_Notify_Default_EMO_Factory::init_svc();
    
	notifyFactory =
	  TAO_Notify_EventChannelFactory_i::create(server.poa.in() ACE_ENV_ARG_PARAMETER);
	ec =
	  notifyFactory->create_channel(initialQos, initialAdmin, id);

	mcAdapter =
	  new Miro::NMC::Adapter(argc, argv, 
				 &server, ec.in(),
				 server.namingContextName);
	mcAdapter->init();
      }
      else 
	ec = 
	  server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel");

      // create local consumers.
      std::vector<PayloadConsumer *> consumer;
      for (int i = 0; i < clients; ++i)
	consumer.push_back(new PayloadConsumer(ec.in(), domainName, i));

      std::cerr << "up and running" << std::endl;
      server.wait();
      std::cerr << "shutting down" << std::endl;
      server.detach(1);

      for (int i = 0; i < clients; ++i) {
	consumer[i]->results();
	//delete consumer[i];
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

