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

#include "FilterGray.h"

#include "video/Parameters.h"
#include "video/VideoServer.h"
#include "video/VideoDeviceDummy.h"
#include "video/VideoFilterBasic.h"
#include "video/VideoFilterRepository.h"

#include "miro/Server.h"
#include "miro/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"

#include <iostream>

int
main(int argc, char *argv[])
{
  int rc = 0;

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Video::Parameters * videoParameters = Video::Parameters::instance();

  try {
    std::cout << "registered filters" << endl;
    
    Video::FilterRepository * repo = Video::FilterRepository::instance();
    repo->registerFilter<Video::DeviceDummy>("DeviceDummy");

    repo->registerFilter<Video::FilterCopy>("FilterCopy");
    repo->registerFilter<FilterGray>("FilterGray");

    std::cout << "Config file processing" << endl;
    
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", * robotParameters);
    config->setSection("Video");
    config->getParameters("Video", * videoParameters);

    std::cout << "  robot parameters:" << endl << *robotParameters << endl;
    std::cout << "  video paramters:" << endl << *videoParameters << endl;
    
    std::cout << "Initialize server daemon." << endl;
    Miro::Server server(argc, argv);

    try {
      Video::Service videoService(server, config);
      
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




