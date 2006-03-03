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
#include "video/Parameters.h"

#include "VideoFilterRepository.h"

#include "miro/Server.h"
#include "idl/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Configuration.h"
#include "miro/Log.h"

int
main(int argc, char *argv[])
{
  int rc = 1;
  try {
    Miro::Log::init(argc, argv);
    Miro::Configuration::init(argc, argv);
  }
  catch (Miro::Exception const & e) {
    std::cerr << "Initialization of logging failed: " << std::endl
	      << e << std::endl;
    return rc;
  }
  
  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  std::vector<Video::Parameters> params;

  try {
    // populating the filter repository
    Miro::VIP::FilterService filters;
    filters.init();

    MIRO_LOG(LL_NOTICE, "Config file processing.");
    
    Miro::ConfigDocument * config = Miro::Configuration::document();
    config->setSection("Robot");
    config->getParameters("Miro::RobotParameters", * robotParameters);
    config->setSection("Video");

    Miro::ConfigDocument::StringVector interfaces = 
      config->getInstances("Video::Parameters");

    Miro::ConfigDocument::StringVector::const_iterator first, last = interfaces.end();
    params.reserve(interfaces.size());
    for (first = interfaces.begin(); first != last; ++first) {
      Video::Parameters videoParameters;
      config->getType("Video::Parameters", *first, videoParameters);
      params.push_back(videoParameters);
    }

    MIRO_LOG_OSTR(LL_NOTICE,
		  "Robot parameters:" << std::endl <<
		  *robotParameters);
    if (Miro::Log::level() >= Miro::Log::LL_NOTICE) {
      for (unsigned i = 0; i < params.size(); ++i) {
	MIRO_LOG_OSTR(LL_NOTICE,
		      "Video interface: " << interfaces[i] << std::endl <<
		      params[i]);
      }
    }

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
    Miro::Server server(argc, argv);

    std::vector<Video::Service *> service;
    try {
      service.reserve(params.size());
      for (unsigned i = 0; i < params.size(); ++i) {
	try {
	  service.push_back(new Video::Service(server, config, &params[i]));
	}
	catch (Miro::Exception const& e) {
	  MIRO_LOG_OSTR(LL_ERROR, 
			"Failed to set up video service " << interfaces[i] << ":\n" << e);
	}
	catch (CORBA::Exception const& e) {
	  MIRO_LOG_OSTR(LL_ERROR, 
			"CORBA exception while initializing video service " << interfaces[i] << ":\n" << e);
	}
      }

      if (service.size() > 0) {
	MIRO_LOG(LL_NOTICE, "Loop forever handling events.");
	server.run(3);
	MIRO_LOG(LL_NOTICE, "Video::Service ended, exiting.");

	for (unsigned i = 0; i < service.size(); ++i) {
	  delete service[i];
	}
      }
      else {
	MIRO_LOG(LL_CRITICAL, "No video service successfully initialized.");
      }
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

    filters.fini();
    rc = 0;
  }
  catch (const Miro::CException& e) {
    MIRO_LOG_OSTR(LL_CRITICAL,
		  "Miro C exception: " << e);
  }
  catch (const Miro::Exception& e) {
    MIRO_LOG_OSTR(LL_CRITICAL,
		  "Miro exception: " << e);
  }
  catch (...) {
    MIRO_LOG(LL_CRITICAL, "Uncaught exception.");
  }
  return rc;
}




