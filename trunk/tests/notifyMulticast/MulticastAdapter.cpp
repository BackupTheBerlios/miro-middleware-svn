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

#include "miro/NotifyMulticast.h"
#include "miro/Server.h"
#include "miro/Configuration.h"
#include "miro/TimeHelper.h"
#include "miro/Log.h"
#include "miro/NotifyMulticastParameters.h"

int main(int argc, char *argv[])
{
  int rc = 1;

  // read syslog options
  Miro::Log::init(argc, argv);

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Miro::NMC::Parameters * nmcParameters = Miro::NMC::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config =  Miro::Configuration::document();
  config->setSection("Robot");
  config->getParameters("Miro::RobotParameters", *robotParameters);
  config->setSection("Notification");
  config->getParameters("Miro::NMC::Parameters", *nmcParameters);
  
  MIRO_DBG_OSTR(MIRO , LL_PRATTLE , "  robot parameters:\n" << robotParameters);
  MIRO_DBG_OSTR(MIRO , LL_PRATTLE , "  multicast parameters:\n" << nmcParameters);

  MIRO_LOG(LL_NOTICE,"Initialize server daemon.");
  Miro::Server server(argc, argv);

  try {
    // The one channel that we create using the factory.
    CosNotifyChannelAdmin::EventChannel_var ec =
      server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel");
    
    Miro::NMC::Adapter mcAdapter(argc, argv, &server, ec, server.namingContextName);
    mcAdapter.init();
  
    try {
      MIRO_LOG(LL_NOTICE , "Loop forever handling events." );
      server.run();
      MIRO_LOG(LL_NOTICE , "Server loop ended, exiting." );
      rc = 0;
    }
    catch (const Miro::Exception& e) {
      MIRO_LOG_OSTR(LL_CRITICAL , "Uncought Miro excepition: " << e);
    }

    server.detach(1);
    mcAdapter.fini();

    MIRO_LOG(LL_NOTICE , "shutting down server.");
    server.shutdown();
    server.wait();
  }
  catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR(LL_CRITICAL , "Uncaught CORBA exception: " << e);
  }
  return rc;
}
