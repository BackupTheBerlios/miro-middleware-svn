// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "FilterGray.h"                      // Custom filter declaration.

#include "FilterDiff.h"                      // Custom filter declaration.

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
  // Return code.
  int rc = 1;
  
  Miro::Log::init(argc, argv);
  Miro::Configuration::init(argc, argv);

  // Miro::Server parameter.
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  // Video service parameters.
  Video::Parameters videoParameters;

  try {
    // Building up the filter repository.
    std::cout << "registered filters" << std::endl;
    
    Video::FilterRepository * repo = Video::FilterRepository::instance();
    repo->registerFilter<Video::DeviceDummy>("DeviceDummy");
    repo->registerFilter<Video::FilterCopy>("FilterCopy");
    repo->registerFilter<FilterGray>("FilterGray");
    repo->registerFilter<FilterDiff>("FilterDiff");

    // Loading the parameters from the configuration file.
    std::cout << "Config file processing" << std::endl;
    
    Miro::ConfigDocument * config = Miro::Configuration::document();
    config->setSection("Robot");
    config->getParameters("Miro::RobotParameters", * robotParameters);
    config->setSection("Video");
    config->getInstance("GrayVideoService", videoParameters);

    // Debug output of the configuration.
    std::cout << "  robot parameters:" << std::endl << *robotParameters << std::endl;
    std::cout << "  video paramters:" << std::endl << videoParameters << std::endl;
    
    std::cout << "Initialize server daemon." << std::endl;
    Miro::Server server(argc, argv);

    try {
      // Building the filter tree and initializing the filters.
      Video::Service videoService(server, config, &videoParameters);
      
      std::cout << "Loop forever handling events." << std::endl;
      server.run(3);
      std::cout << "videoService ended, exiting." << std::endl;

      rc = 0;
    }
    catch (const Miro::EOutOfBounds& e) {
      std::cerr << "OutOfBounds excetpion: Wrong parameter for device initialization." << std::endl;
      rc = 1;
    }
    catch (const Miro::EDevIO& e) {
      std::cerr << "DevIO excetpion: Device access failed." << std::endl;
      rc = 1;
    }
    catch (const CORBA::Exception & e) {
      std::cerr << "Uncaught CORBA exception: " << e << std::endl;
      rc = 1;
    }
  }
  catch (const Miro::Exception& e) {
    std::cerr << "Miro exception: " << e << std::endl;
    rc = 1;
  }
  catch (...) {
    std::cerr << "Uncaught exception: " << std::endl;
    rc = 1;
  }
  return rc;
}




