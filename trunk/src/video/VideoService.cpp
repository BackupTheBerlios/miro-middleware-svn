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

#include "Parameters.h"

#include "VideoDevice.h"
#ifdef MIRO_HAS_BTTV
#include "VideoDeviceBTTV.h"
#endif
#ifdef MIRO_HAS_METEOR
#include "VideoDeviceMeteor.h"
#endif
#ifdef MIRO_HAS_1394
#include "VideoDevice1394.h"
#include "VideoFilterYUV422toYUV.h"
#include "VideoFilterYUV422toRGB.h"
#include "VideoFilterYUV411toRGB.h"
#include "VideoFilterYUV411toYUV.h"
#endif
#ifdef MIRO_HAS_QUICKCAM
#include "VideoDeviceQuickCam.h"
#include "VideoFilterYUV420PtoRGB.h"
#include "VideoFilterOmni2Pan.h"
#include "VideoFilterRGBtoSeg.h"
#endif
#include "VideoDeviceDummy.h"

#include "VideoFilterRepository.h"
#include "VideoFilterBasic.h"

#include "miro/Server.h"
#include "idl/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Log.h"

int
main(int argc, char *argv[])
{
  int rc = 0;
  try {
    Miro::Log::init(argc, argv);
  }
  catch (Miro::Exception const & e) {
    std::cerr << "Initialization of logging failed: " << std::endl
	      << e << std::endl;
    return 1;
  }

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Video::Parameters * videoParameters = Video::Parameters::instance();

  try {
    MIRO_LOG(LL_NOTICE, "Registered filters.");

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

#ifdef MIRO_HAS_QUICKCAM
    repo->registerFilter<Video::DeviceQuickCam>("DeviceQuickCam");
    repo->registerFilter<Video::FilterYUV420PtoRGB>("FilterYUV420PtoRGB");
    repo->registerFilter<Video::FilterOmni2Pan>("FilterOmni2Pan");
    repo->registerFilter<Video::FilterRGBtoSeg>("FilterRGBtoSeg");
#endif

    repo->registerFilter<Video::FilterCopy>("FilterCopy");
    repo->registerFilter<Video::FilterSwap3>("FilterSwap3");
    repo->registerFilter<Video::FilterSwap4>("FilterSwap4");
    repo->registerFilter<Video::FilterFlip>("FilterFlip");
    repo->registerFilter<Video::FilterReverse>("FilterReverse");
    repo->registerFilter<Video::FilterHalfImage>("FilterHalfImage");

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
    }
    catch (const Miro::EOutOfBounds& e) {
      MIRO_LOG(LL_CRITICAL,
	       "OutOfBounds excetpion: Wrong parameter for device initialization.");
      rc = 1;
    }
    catch (const Miro::EDevIO& e) {
      MIRO_LOG(LL_CRITICAL,
	       "DevIO excetpion: Device access failed.");
      rc = 1;
    }
    catch (const CORBA::Exception & e) {
      MIRO_LOG_OSTR(LL_CRITICAL,
		    "Uncaught CORBA exception: " << e);
      rc = 1;
    }
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




