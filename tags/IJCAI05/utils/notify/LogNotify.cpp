// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c)  2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/LogNotifyConsumer.h"
#include "miro/Server.h"
#include "miro/ConfigDocument.h"
#include "miro/TimeHelper.h"
#include "miro/Log.h"

int
main(int argc, char *argv[])
{
  int rc = 1;
  std::string fileName;

  // read syslog options
  Miro::Log::init(argc, argv);

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Miro::LogNotifyParameters * parameters = Miro::LogNotifyParameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = 
    new Miro::ConfigDocument(argc, argv);
  config->setSection("Robot");
  config->getParameters("Robot", *robotParameters);
  config->setSection("Notification");
  config->getParameters("Logging", *parameters);
  delete config;
  
  MIRO_DBG_OSTR(MIRO , LL_PRATTLE , "  robot parameters:\n" << robotParameters);
  MIRO_DBG_OSTR(MIRO , LL_PRATTLE , "  logging parameters:\n" << parameters);

  MIRO_LOG(LL_NOTICE,"Initialize server daemon.");
  Miro::Server server(argc, argv);

  // override default logfile name with command line parameter
  if (argc > 1)
    fileName = argv[1];

  try {
    // The one channel that we create using the factory.
    CosNotifyChannelAdmin::EventChannel_var 
      ec(server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel"));
    
    // The consumer, that gets the events
    Miro::LogNotifyConsumer pushConsumer(server,
					 ec.in(), 
					 server.namingContextName,
					 fileName,
					 *parameters,
					 false);
    
    try {
      MIRO_LOG(LL_NOTICE , "Loop forever handling events." );
      server.run(1);
      MIRO_LOG(LL_NOTICE , "Server loop ended, exiting." );
      rc = 0;
    }
    catch (const Miro::Exception& e) {
      MIRO_LOG_OSTR(LL_CRITICAL , "Uncought Miro excepition: " << e);
    }

    server.detach(1);

    MIRO_LOG(LL_NOTICE , "shutting down server.");
    pushConsumer.disconnect();
    server.shutdown();
    server.wait();
  }
  catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR(LL_CRITICAL , "Uncaught CORBA exception: " << e);
  }
  return rc;
}
