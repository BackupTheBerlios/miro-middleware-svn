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
#include "FilterGray.h"                      // Custom filter declaration.
#include "FilterDiff.h"                      // Custom filter declaration.
#include "FilterBusy.h"                      // Custom filter declaration.

#include "video/VideoServer.h"               // VideoServer declaration.
#include "video/VideoDeviceDummy.h"          // Dummy device declaration.
#include "video/VideoFilterBasic.h"          // FilterCopy declaration.
#include "video/VideoFilterRepository.h"     // FilterRepository declaration.

#include "miro/Server.h"                     // Server declaration.
#include "idl/ExceptionC.h"                 // CORBA exceptions.
#include "miro/Exception.h"                  // Miro exceptionss
#include "miro/Configuration.h"              // ConfigDocumet declaration.

#include <iostream>


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
    Video::FilterRepository * repo = Video::FilterRepository::instance();
    repo->registerFilter<Video::DeviceDummy>("DeviceDummy");
    repo->registerFilter<Video::FilterCopy>("FilterCopy");
    repo->registerFilter<FilterGray>("FilterGray");
    repo->registerFilter<FilterDiff>("FilterDiff");
    repo->registerFilter<FilterBusy>("FilterBusy");

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


