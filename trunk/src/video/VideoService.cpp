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

#include "VideoFilterRepository.h"

#include "miro/Server.h"
#include "idl/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Log.h"


#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>


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
  
#ifdef MIRO_HAS_QUICKCAM
  TAO_Notify_Default_CO_Factory::init_svc();
  TAO_Notify_Default_POA_Factory::init_svc();
  TAO_Notify_Default_Collection_Factory::init_svc();
  TAO_Notify_Default_EMO_Factory::init_svc();
#endif

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Video::Parameters * videoParameters = Video::Parameters::instance();
  Video::Parameters * videoParametersOmniCam = new Video::Parameters();

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
#ifdef MIRO_HAS_QUICKCAM
    config->getParameters("VideoOmniCam", *videoParametersOmniCam);
#endif

    if (Miro::Log::level() >= Miro::Log::LL_NOTICE) {
      std::cout << "Robot parameters:" << std::endl 
		<< *robotParameters << std::endl;
      std::cout << "Video paramters:" << std::endl 
		<< *videoParameters << std::endl;
    }

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
    Miro::Server server(argc, argv);

#ifdef MIRO_HAS_QUICKCAM    
    CosNotification::QoSProperties initialQos_;
    CosNotification::AdminProperties initialAdmin_;
    
    CosNotifyChannelAdmin::EventChannelFactory_ptr notifyFactory_(TAO_Notify_EventChannelFactory_i::create(server.poa.in() ACE_ENV_ARG_PARAMETER));
    CosNotifyChannelAdmin::ChannelID id_;
    CosNotifyChannelAdmin::EventChannel_ptr ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_));
//    std::cout << "Hallo hier bin ich noch" << std::endl;
    server.addToNameService(ec_, "EventChannel");
#endif

    try {
      Video::Service videoService(server, config);
#ifdef MIRO_HAS_QUICKCAM
      Video::Service videoServiceOmniCam(server, config, videoParametersOmniCam);
#endif     
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




