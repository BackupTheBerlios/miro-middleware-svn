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


#include "SparrowServer.h"

#include "miro/Exception.h"
#include "miro/Utils.h"

#include "sparrow/SparrowParameters.h"
#include "pioneer/PioneerParameters.h"
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

char hostName[256];

int
main(int argc, char *argv[])
{
  int rc = 0;

  // Init TAO Factories
  TAO_Notify_Default_CO_Factory::init_svc();
  TAO_Notify_Default_POA_Factory::init_svc();
  TAO_Notify_Default_Collection_Factory::init_svc();
  TAO_Notify_Default_EMO_Factory::init_svc();

  // Parameters to be passed to the services
  Miro::RobotParameters robotParameters;
  Sparrow::Parameters * pSparrowParameters = Sparrow::Parameters::instance();
  Pioneer::Parameters * pPioneerParameters = Pioneer::Parameters::instance();
  Miro::NotifyMulticast::Parameters * notifyMulticastParameters = Miro::NotifyMulticast::Parameters::instance();

  try {
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setRobotType("Robot");
    config->getParameters("robot", robotParameters);
    config->setRobotType("Sparrow99");
    config->getParameters("sparrowBoard", *pSparrowParameters);
    config->getParameters("pioneerBoard", *pPioneerParameters);
    config->setRobotType("Notification");
    config->getParameters("NotifyMulticast", *notifyMulticastParameters);
    delete config;

#ifdef DEBUG
    cout << "  robot paramters:" << endl << robotParameters << endl;
    cout << "  sparrow paramters:" << endl << *pSparrowParameters << endl;
    cout << "  pioneer paramters:" << endl << *pPioneerParameters << endl;
    cout << "  NotifyMulticast parameters: " << endl << *notifyMulticastParameters << endl;
#endif

    DBG(cout << "Initialize server daemon." << endl);
    SparrowBase sparrowBase(argc, argv, 
			    robotParameters);
    try {
      DBG(cout << "Loop forever handling events." << endl);
      sparrowBase.run(8);
      DBG(cout << "sparrowBase ended, exiting." << endl);
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




