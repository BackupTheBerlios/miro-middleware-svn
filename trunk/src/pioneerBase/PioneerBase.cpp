// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PioneerBase.h"

#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Synch.h"

#include "pioneer/PioneerParameters.h"

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

PioneerBase::PioneerBase(int argc, char *argv[]) :
  Super(argc, argv),
  reactorTask(this),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  odometry(&structuredPushSupplier_),
  battery(),

  // Pioneer board initialization
  pPioneerConsumer(new Pioneer::Consumer(&sonar, 
					 &tactile, 
					 &odometry, 
					 &battery,
					 NULL, //stall
					 //only add the camera if really present
					 (Pioneer::Parameters::instance()->camera?&canonPanTilt:NULL))),
  pPsosEventHandler(new Psos::EventHandler(pPioneerConsumer, pioneerConnection)),
  pioneerConnection(reactorTask.reactor(), pPsosEventHandler, pPioneerConsumer),

  // Service initialization
  motion(pioneerConnection, *pPioneerConsumer),
  stall(/*pioneerConnection*/),
  sonar(Pioneer::Parameters::instance()->sonarDescription, &structuredPushSupplier_),
  tactile(Pioneer::Parameters::instance()->tactileDescription, &structuredPushSupplier_),
  canonPanTilt(pioneerConnection, *pPioneerConsumer,Pioneer::Parameters::instance()->cameraUpsideDown),
  canonCamera(pioneerConnection, *pPioneerConsumer, canonPanTilt.getAnswer()),
  gripper(pioneerConnection, *pPioneerConsumer)
{
  pOdometry = odometry._this();
  pMotion = motion._this();
  pStall = stall._this();
  pSonar = sonar._this();
  pTactile = tactile._this();
  pBattery = battery._this();
  pCanonPanTilt = canonPanTilt._this();
  pCanonCamera = canonCamera._this();
  pGripper = gripper._this();

  addToNameService(pOdometry.in(), "Odometry");
  addToNameService(pMotion.in(), "Motion");
  addToNameService(pStall.in(), "Stall");
  addToNameService(pSonar.in(), "Sonar");
  addToNameService(pTactile.in(), "Tactile");
  addToNameService(pBattery.in(), "Battery");
  addToNameService(ec_.in(), "EventChannel");

  //only add the pantilt if the camera is actually present
  if (Pioneer::Parameters::instance()->camera) {
    addToNameService(pCanonPanTilt.in(), "PanTilt");
    addToNameService(pCanonPanTilt.in(), "Camera");
  }

  addToNameService(pGripper.in(), "Gripper");

  // start the asychronous consumer listening for the hardware
  reactorTask.open(0);

  DBG(cout << "PioneerBase initialized.." << endl);
}

PioneerBase::~PioneerBase()
{
  DBG(cout << "Destructing PioneerBase." << endl);

  pioneerConnection.close();

  // Deactivate.
  PortableServer::ObjectId_var oid =
    poa->reference_to_id (notifyFactory_.in());

  // deactivate from the poa.
  poa->deactivate_object (oid.in ());

  reactorTask.cancel();
  DBG(cout << "reactor Task ended" << endl);
}

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
  Pioneer::Parameters * pioneerParameters = Pioneer::Parameters::instance();

  try {
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setRobotType("Pioneer1");
    config->getParameters("pioneerBoard", *pioneerParameters);
    delete config;
    
#ifdef DEBUG
    cout << "  pioneer parameters:" << endl << *pioneerParameters << endl;
#endif
    
    DBG(cout << "Initialize server daemon." << endl);
    PioneerBase pioneerBase(argc, argv);
    try {
      DBG(cout << "Loop forever handling events." << endl);
      pioneerBase.run(8);
      DBG(cout << "pioneerBase ended, exiting." << endl);
    }
    catch (const Miro::EOutOfBounds& e) {
      cerr << "OutOfBounds exception: Wrong parameter for device initialization." << endl;
    }
    catch (const Miro::EDevIO& e) {
      cerr << "DevIO exception: Device access failed." << endl;
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




