// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "VideoServer.h"

#include "miro/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"

#include "video/Parameters.h"

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;

int
main(int argc, char *argv[])
{
  int rc = 0;

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Video::Parameters * videoParameters = Video::Parameters::instance();

  try {
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", * robotParameters);
    config->setSection("Video");
    config->getParameters("Video", * videoParameters);
    delete config;
    
#ifdef DEBUG
    cout << "  robot parameters:" << endl << *robotParameters << endl;
    cout << "  video paramters:" << endl << *videoParameters << endl;
#endif
    
    DBG(cout << "Initialize server daemon." << endl);
    VideoService videoService(argc, argv);
    try {
      DBG(cout << "Loop forever handling events." << endl);
      videoService.run(3);
      DBG(cout << "videoService ended, exiting." << endl);
    }
    catch (const Miro::EOutOfBounds& e) {
      cerr << "OutOfBounds excetpion: Wrong parameter for device initialization." << endl;
    }
    catch (const Miro::EDevIO& e) {
      cerr << "DevIO excetpion: Device access failed." << endl;
    }
    catch (const CORBA::Exception & e) {
      cerr << "Uncaught CORBA exception: " << e << endl;
      rc = 1;
    }
  }
  catch (const Miro::CException& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
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




