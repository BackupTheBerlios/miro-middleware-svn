// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "VideoServer.h"
#include "VideoFilterService.h"
#include "Parameters.h"

#include "miro/Server.h"
#include "idl/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Log.h"

int
main(int argc, char *argv[])
{
  int rc = 1;
  try {
    Miro::Log::init(argc, argv);
  }
  catch (Miro::Exception const & e) {
    std::cerr << "Initialization of logging failed: " << std::endl
	      << e << std::endl;
    return rc;
  }

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Video::Parameters * videoParameters = Video::Parameters::instance();

  try {
    // populating the filter repository
    Miro::VIP::FilterService service;
    service.init();

    MIRO_LOG(LL_NOTICE, "Config file processing.");
    
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", * robotParameters);
    config->setSection("Video");
    config->getParameters("Video", * videoParameters);

    if (Miro::Log::level() >= Miro::Log::LL_NOTICE) {
      std::cout << "Robot parameters:" << std::endl 
		<< *robotParameters << std::endl;
      std::cout << "Video paramters:" << std::endl 
		<< *videoParameters << std::endl;
    }

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
    Miro::Server server(argc, argv);

    try {
      Video::Service videoService(server, config);
      
      delete config;

      MIRO_LOG(LL_NOTICE, "Loop forever handling events.");
      server.run(3);
      MIRO_LOG(LL_NOTICE, "videoService ended, exiting.");
      rc = 0;
    }
    catch (const Miro::EOutOfBounds& e) {
      MIRO_LOG(LL_CRITICAL,
	       "OutOfBounds excetpion: Wrong parameter for device initialization.");
    }
    catch (const Miro::EDevIO& e) {
      MIRO_LOG(LL_CRITICAL,
	       "DevIO excetpion: Device access failed.");
    }
    catch (const CORBA::Exception & e) {
      MIRO_LOG_OSTR(LL_CRITICAL,
		    "Uncaught CORBA exception: " << e);
    }

    service.fini();
  }
  catch (const Miro::CException& e) {
    MIRO_LOG_OSTR(LL_CRITICAL,
		  "Miro C exception: " << e);
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    MIRO_LOG_OSTR(LL_CRITICAL,
		  "Miro exception: " << e);
    rc = 1;
  }
  catch (...) {
    MIRO_LOG(LL_CRITICAL, "Uncaught exception.");
    rc = 1;
  }
  return rc;
}




