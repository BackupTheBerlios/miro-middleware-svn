// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SparrowServer.h"

#include "miro/Configuration.h"
#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Log.h"
#include "miro/IO.h"

#include "sparrow/Parameters.h"
#include "faulMotor/Parameters.h"
#include "miro/NotifyMulticastParameters.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

int
main(int argc, char *argv[])
{
  int rc = 1;
  // Init TAO Factories
  TAO_Notify_Default_CO_Factory::init_svc();
  TAO_Notify_Default_POA_Factory::init_svc();
  TAO_Notify_Default_Collection_Factory::init_svc();
  TAO_Notify_Default_EMO_Factory::init_svc();
    
  try {
    Miro::Log::init(argc, argv);
    Miro::Configuration::init(argc, argv);

    // Parameters to be passed to the services
    Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
    Sparrow::Parameters * pSparrowParameters = Sparrow::Parameters::instance();
    FaulMotor::Parameters * pFaulMotorParameters = NULL;
    Miro::NMC::Parameters * notifyMulticastParameters = Miro::NMC::Parameters::instance();
    
    // Config file processing
    Miro::ConfigDocument * config =  Miro::Configuration::document();
    config->setSection("Robot");
    config->getParameters("Miro::RobotParameters", *robotParameters);

    config->setSection("Sparrow");
    config->getParameters("Sparrow::Parameters", *pSparrowParameters);

    if (pSparrowParameters->faulhaber) {
      pFaulMotorParameters = FaulMotor::Parameters::instance();
      config->setSection("Faulhaber");
      config->getParameters("FaulMotor::Parameters", *pFaulMotorParameters);
    }
    config->setSection("Notification");
    config->getParameters("Miro::NMC::Parameters", *notifyMulticastParameters);
    config->fini();

    MIRO_LOG_OSTR(LL_NOTICE,
		  "  robot paramters:\n" << robotParameters <<
		  "\n  sparrow paramters:\n" << *pSparrowParameters);
    if (pFaulMotorParameters)
      MIRO_LOG_OSTR(LL_NOTICE,
		    "\n  faulhaber paramters:\n" << *pFaulMotorParameters);
    MIRO_LOG_OSTR(LL_NOTICE,
		  "  NotifyMulticast parameters:\n" << *notifyMulticastParameters);

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
    Miro::Server server(argc, argv);
    try {
      SparrowBase sparrowBase(server);
	     
      MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
      server.run(4);
      MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
      server.detach(2);

      rc = 0;
    }
    catch (Miro::EOutOfBounds const& e) {
          MIRO_LOG_OSTR(LL_CRITICAL, 
			"OutOfBounds excetpion: Wrong parameter for device initialization.\n" <<
			e);
    }
    catch (Miro::EDevIO const& e) {
          MIRO_LOG_OSTR(LL_CRITICAL, 
			"DevIO excetpion: Device access failed.\n" << e);
    }
    catch (CORBA::Exception const& e) {
          MIRO_LOG_OSTR(LL_CRITICAL, 
			"Uncaught CORBA exception:\n" << e);
    }
    server.shutdown();
    server.wait();
  }
  catch (Miro::Exception const& e) {
          MIRO_LOG_OSTR(LL_CRITICAL, 
			"Miro exception:\n" << e);
    rc = 1;
  }
  return rc;
}




