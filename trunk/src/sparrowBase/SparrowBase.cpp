// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SparrowServer.h"

#include "miro/Exception.h"
#include "miro/Utils.h"

#include "sparrow/Parameters.h"
#include "faulMotor/Parameters.h"
#include "miro/NotifyMulticastParameters.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;
using std::endl;

char hostName[256];

int
main(int argc, char *argv[])
{
  int rc = 0;
  try {
    Miro::Log::init(argc, argv);

    // Init TAO Factories
    TAO_Notify_Default_CO_Factory::init_svc();
    TAO_Notify_Default_POA_Factory::init_svc();
    TAO_Notify_Default_Collection_Factory::init_svc();
    TAO_Notify_Default_EMO_Factory::init_svc();
    
    // Parameters to be passed to the services
    Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
    Sparrow::Parameters * pSparrowParameters = Sparrow::Parameters::instance();
    FaulMotor::Parameters * pFaulMotorParameters = NULL;
    Miro::NMC::Parameters * notifyMulticastParameters = Miro::NMC::Parameters::instance();
    
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", *robotParameters);

    config->setSection("Sparrow99");
    config->getParameters("SparrowBoard", *pSparrowParameters);

    if (pSparrowParameters->faulhaber) {
      pFaulMotorParameters = FaulMotor::Parameters::instance();
      config->setSection("Faulhaber");
      config->getParameters("FaulMotor", *pFaulMotorParameters);
    }
    config->setSection("Notification");
    config->getParameters("NotifyMulticast", *notifyMulticastParameters);
    delete config;

#ifdef DEBUG
    cout << "  robot paramters:" << endl << robotParameters << endl;
    cout << "  sparrow paramters:" << endl << *pSparrowParameters << endl;
    if (pFaulMotorParameters)
      cout << "  faulhaber paramters:" << endl << *pFaulMotorParameters << endl;
    cout << "  NotifyMulticast parameters: " << endl << *notifyMulticastParameters << endl;
#endif

    DBG(cout << "Initialize server daemon." << endl);
    Miro::Server server(argc, argv);
    try {
      SparrowBase sparrowBase(server);
	     
      DBG(cout << "Loop forever handling events." << endl);
      server.run(4);
      DBG(cout << "sparrowBase ended, exiting." << endl);
      server.detach(2);
    }
    catch (const Miro::EOutOfBounds& e) {
      cerr << "OutOfBounds excetpion: Wrong parameter for device initialization." << endl;
      rc = 1;
    }
    catch (const Miro::EDevIO& e) {
      cerr << "DevIO excetpion: Device access failed." << endl;
      rc = 1;
    }
    catch (const CORBA::Exception & e) {
      cerr << "Uncaught CORBA exception: " << e << endl;
      rc = 1;
    }
    server.shutdown();
    server.wait();
  }
  catch (const Miro::Exception& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }
  return rc;
}




