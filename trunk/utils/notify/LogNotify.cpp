// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "miro/LogNotifyConsumer.h"
#include "miro/Server.h"
#include "miro/Configuration.h"
#include "miro/TimeHelper.h"
#include "miro/Log.h" 

int
main(int argc, char *argv[])
{
  int rc = 1;
  std::string fileName;

  // read syslog options
  Miro::Log::init(argc, argv);
  Miro::Configuration::init(argc, argv);

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Miro::LogNotifyParameters * parameters = Miro::LogNotifyParameters::instance();

  // Config file processing
  Miro::ConfigDocument * config =  Miro::Configuration::document();
  config->setSection("Robot");
  config->getParameters("Miro::RobotParameters", *robotParameters);
  config->setSection("Notification");
  config->getParameters("Miro::LogNotifyParameters", *parameters);

  
  MIRO_DBG_OSTR(MIRO , LL_PRATTLE , "  robot parameters:\n" << *robotParameters);
  MIRO_DBG_OSTR(MIRO , LL_PRATTLE , "  logging parameters:\n" << *parameters);

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
