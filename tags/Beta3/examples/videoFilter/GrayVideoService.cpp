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
#include "miro/Utils.h"                      // ConfigDocumet declaration.

#include <iostream>

int
main(int argc, char *argv[])
{
  // Return code.
  int rc = 0;

  // Miro::Server parameter.
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  // Video service parameters.
  Video::Parameters * videoParameters = Video::Parameters::instance();

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
    
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", * robotParameters);
    config->setSection("Video");
    config->getParameters("Video", * videoParameters);

    // Debug output of the configuration.
    std::cout << "  robot parameters:" << std::endl << *robotParameters << std::endl;
    std::cout << "  video paramters:" << std::endl << *videoParameters << std::endl;
    
    std::cout << "Initialize server daemon." << std::endl;
    Miro::Server server(argc, argv);

    try {
      // Building the filter tree and initializing the filters.
      Video::Service videoService(server, config);
      
      // We don't need the config file anymore.
      delete config;

      std::cout << "Loop forever handling events." << std::endl;
      server.run(3);
      std::cout << "videoService ended, exiting." << std::endl;
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




