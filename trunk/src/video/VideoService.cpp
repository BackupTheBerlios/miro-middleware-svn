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

#include "VideoServer.h"

#include "miro/Server.h"
#include "miro/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"

#include "Parameters.h"

#include "VideoDevice.h"
#include "VideoDeviceBTTV.h"
#include "VideoDeviceMeteor.h"
#include "VideoDevice1394.h"
#include "VideoDeviceDummy.h"

#include "VideoFilterRepository.h"
#include "VideoFilterBasic.h"
#include "VideoFilterYUV422toYUV.h"
#include "VideoFilterYUV422toRGB.h"
#include "VideoFilterYUV411toRGB.h"
#include "VideoFilterYUV411toYUV.h"

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
    std::cout << "registered filters" << endl;
    
    Video::FilterRepository * repo = Video::FilterRepository::instance();
    repo->registerFilter<Video::DeviceDummy>("DeviceDummy");

#ifdef MIRO_HAS_METEOR
    repo->registerFilter<Video::DeviceMeteor>("DeviceMeteor");
#endif

#ifdef MIRO_HAS_BTTV
    repo->registerFilter<Video::DeviceBTTV>("DeviceBTTV");
#endif

#ifdef MIRO_HAS_1394
    repo->registerFilter<Video::Device1394>("Device1394");
    repo->registerFilter<Video::FilterYUV422toRGB>("FilterYUV422toRGB");
    repo->registerFilter<Video::FilterYUV422toYUV>("FilterYUV422toYUV");
    repo->registerFilter<Video::FilterYUV411toRGB>("FilterYUV411toRGB");
    repo->registerFilter<Video::FilterYUV411toYUV>("FilterYUV411toYUV");
#endif

    repo->registerFilter<Video::FilterCopy>("FilterCopy");
    repo->registerFilter<Video::FilterSwap3>("FilterSwap3");
    repo->registerFilter<Video::FilterSwap4>("FilterSwap4");
    repo->registerFilter<Video::FilterFlip>("FilterFlip");

    std::cout << "Config file processing" << endl;
    
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", * robotParameters);
    config->setSection("Video");
    config->getParameters("Video", * videoParameters);

#ifdef DEBUG
    cout << "  robot parameters:" << endl << *robotParameters << endl;
    cout << "  video paramters:" << endl << *videoParameters << endl;
#endif
    
    DBG(cout << "Initialize server daemon." << endl);
    Miro::Server server(argc, argv);

    try {
      Video::Service videoService(server, config);
      
      delete config;

      DBG(cout << "Loop forever handling events." << endl);
      server.run(3);
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




