// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "B21Base.h"

#include "miro/Exception.h"
#include "miro/Utils.h"

#include "base/BaseImpl.h"
#include "base/BaseConsumer.h"
#include "base/Parameters.h"

#include "msp/Parameters.h"

#include "buttons/ButtonsEventHandler.h"

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

using CosNotifyChannelAdmin::EventChannelFactory;

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"

CosNaming::Name channelFactoryName;

B21Base::B21Base(int& argc, char *argv[]) :
  Super(argc, argv),
  reactorTask(this),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName.c_str()),

  pMspConsumer(new Msp::Consumer(mspConnection,
				 &sonar,
				 &infrared,
				 &tactile)),
  pMspEventHandler(new Msp::EventHandler(mspConnection, pMspConsumer)),
  mspConnection(reactorTask.reactor(), pMspEventHandler),

  pB21ButtonsEventHandler(new B21Buttons::EventHandler(b21ButtonsConnection, structuredPushSupplier_)),
  b21ButtonsConnection(reactorTask.reactor(), pB21ButtonsEventHandler),

  odometry(&structuredPushSupplier_),
  b21Motion(reactorTask.reactor(), &odometry),
  infrared(Msp::Parameters::instance()->infraredDescription, &structuredPushSupplier_),
  sonar(Msp::Parameters::instance()->sonarDescription, &structuredPushSupplier_),
  tactile(Msp::Parameters::instance()->tactileDescription, &structuredPushSupplier_),
  b21Buttons(b21ButtonsConnection, pB21ButtonsEventHandler)
{
  CosNaming::Name n(1);
  n.length(1);

  Miro::Odometry_var pOdometry = odometry._this();
  Miro::B21Motion_var pB21Motion = b21Motion._this();
  Miro::RangeSensor_var pInfrared = infrared._this();
  Miro::RangeSensor_var pSonar = sonar._this();
  Miro::RangeSensor_var pTactile = tactile._this();
  Miro::B21Buttons_var pB21Buttons = b21Buttons._this();

  addToNameService(pOdometry.in(), "Odometry");
  addToNameService(pB21Motion.in(), "Motion");
  addToNameService(pInfrared.in(), "Infrared");
  addToNameService(pSonar.in(), "Sonar");
  addToNameService(pTactile.in(), "Tactile");
  addToNameService(pB21Buttons.in(), "Buttons");
  addToNameService(ec_.in(), "EventChannel");

  // start the asychronous consumer listening for the hardware
  reactorTask.open(0);

  b21Motion.setStatusReportPeriod(26); // 1/10 sec
  mspConnection.sonarStart();

  DBG(cout << "B21Base initialized.." << endl);
}

B21Base::~B21Base()
{
  DBG(cout << "Destructing B21Base." << endl);

  mspConnection.sonarStop();

  reactorTask.cancel();
}

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
    Base::Parameters * baseParameters = Base::Parameters::instance();
    Msp::Parameters * mspParameters = Msp::Parameters::instance();
    
    // Config file processing
    Miro::ConfigDocument *config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", *robotParameters);
    config->setSection("B21");
    config->getParameters("Base", *baseParameters);
    config->getParameters("Msp", *mspParameters);
    delete config;

#ifdef DEBUG
    cout << "  robot parameters:" << endl << robotParameters << endl;
    cout << "  base parameters:" << endl << baseParameters << endl;
    cout << "  msp parameters:" << endl << mspParameters << endl;
#endif

    DBG(cout << "Initialize server daemon." << endl);
    B21Base b21Base(argc, argv);
    try {
      DBG(cout << "Loop forever handling events." << endl);
      b21Base.run(8);
      DBG(cout << "b21Base ended, exiting." << endl);
    }
    catch (const Miro::EOutOfBounds& e) {
      cerr << "OutOfBounds exception: Wrong parameter for device initialization." << endl;
      rc = 1;
    }
    catch (const Miro::EDevIO& e) {
      cerr << "DevIO exception: Device access failed." << endl;
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
  return rc;
}

