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

#include "video/VideoServer.h"               // VideoServer declaration.
#include "video/VideoDeviceDummy.h"          // Dummy device declaration.
#include "video/VideoFilterBasic.h"          // FilterCopy declaration.
#include "video/VideoFilterRepository.h"     // FilterRepository declaration.

#include "miro/Server.h"                     // Server declaration.
#include "miro/ExceptionC.h"                 // CORBA exceptions.
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
    std::cout << "registered filters" << endl;
    
    Video::FilterRepository * repo = Video::FilterRepository::instance();
    repo->registerFilter<Video::DeviceDummy>("DeviceDummy");
    repo->registerFilter<Video::FilterCopy>("FilterCopy");
    repo->registerFilter<FilterGray>("FilterGray");

    // Loading the parameters from the configuration file.
    std::cout << "Config file processing" << endl;
    
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", * robotParameters);
    config->setSection("Video");
    config->getParameters("Video", * videoParameters);

    // Debug output of the configuration.
    std::cout << "  robot parameters:" << endl << *robotParameters << endl;
    std::cout << "  video paramters:" << endl << *videoParameters << endl;
    
    std::cout << "Initialize server daemon." << endl;
    Miro::Server server(argc, argv);

    try {
      // Building the filter tree and initializing the filters.
      Video::Service videoService(server, config);
      
      // We don't need the config file anymore.
      delete config;

      std::cout << "Loop forever handling events." << endl;
      server.run(3);
      std::cout << "videoService ended, exiting." << endl;
    }
    catch (const Miro::EOutOfBounds& e) {
      std::cerr << "OutOfBounds excetpion: Wrong parameter for device initialization." << endl;
      rc = 1;
    }
    catch (const Miro::EDevIO& e) {
      std::cerr << "DevIO excetpion: Device access failed." << endl;
      rc = 1;
    }
    catch (const CORBA::Exception & e) {
      cerr << "Uncaught CORBA exception: " << e << endl;
      rc = 1;
    }
  }
  catch (const Miro::Exception& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    rc = 1;
  }
  return rc;
}




