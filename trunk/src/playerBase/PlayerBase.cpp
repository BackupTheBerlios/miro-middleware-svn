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

#include "PlayerBase.h"

#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Synch.h"

#include "Parameters.h"

#include "miro/OdometryTracking.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

#include <playerclient.h>

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::string;

PlayerBase::PlayerBase(int argc, char *argv[],PlayerClient* client) throw (CORBA::Exception) :
  Super(argc, argv),
  reactorTask(client,&sonar,&laser,&infrared,&tactile,&odometry,&battery,&motion),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  // Service initialization
  odometry(&structuredPushSupplier_),
  battery(),
  motion(Player::Parameters::instance()->motion),
  //  stall(/*pioneerConnection*/),
    sonar(Player::Parameters::instance()->sonarDescription, &structuredPushSupplier_),
  tactile(Player::Parameters::instance()->tactileDescription, &structuredPushSupplier_),
  //TODO: infrared, not sonar description!
  infrared(Player::Parameters::instance()->sonarDescription, &structuredPushSupplier_),
  laser(new Miro::OdometryTracking(ec_.in(), namingContextName),
	Player::Parameters::instance()->laserDescription, 
    	&structuredPushSupplier_),
  playerClient(client)
{ 

  pOdometry = odometry._this();
  pMotion = motion._this();
  //  pStall = stall._this();
  pSonar = sonar._this();
  pTactile = tactile._this();
  pInfrared = infrared._this();
  pLaser = laser._this();
  pBattery = battery._this();
  //  pCanonPanTilt = canonPanTilt._this();
  //  pCanonCamera = canonCamera._this();
  //  pGripper = gripper._this();

  addToNameService(pOdometry.in(), "Odometry");
  addToNameService(pMotion.in(), "Motion");
  //  if (reactorTask.stallBound())
  //    addToNameService(pStall.in(), "Stall");
  if (reactorTask.sonarBound())
    addToNameService(pSonar.in(), "Sonar");
  if (reactorTask.tactileBound())
    addToNameService(pTactile.in(), "Tactile");
  if (reactorTask.infraredBound())
    addToNameService(pInfrared.in(), "Infrared");
  if (reactorTask.laserBound())
    addToNameService(pLaser.in(),"Laser");
  if (reactorTask.batteryBound())
    addToNameService(pBattery.in(), "Battery");
  addToNameService(ec_.in(), "EventChannel");

  //only add the pantilt if the camera is actually present
  //  if (Player::Parameters::instance()->camera) {
  //    addToNameService(pCanonPanTilt.in(), "PanTilt");
  //    addToNameService(pCanonPanTilt.in(), "Camera");
  //  }

  //  addToNameService(pGripper.in(), "Gripper");

  // start the asychronous consumer listening for the hardware
  reactorTask.open(0);

  DBG(cout << "PlayerBase initialized.." << endl);
}

PlayerBase::~PlayerBase()
{
  DBG(cout << "Destructing PlayerBase." << endl);

  try {
    // Deactivate.
    PortableServer::ObjectId_var oid =
      poa->reference_to_id (notifyFactory_.in());
    
    // deactivate from the poa.
    poa->deactivate_object (oid.in ());
  }
  catch (const CORBA::Exception & e) {
    cout << "CORBA::Exception: " << e << endl;
  }
  catch (const Miro::Exception & e) {
    cout << "Miro::Exception: " << e << endl;
  }
  catch (const std::string & e) {
    cout << "string exception: " << e << endl;
  }
  catch (...) {
    //TODO: find out what the hell is this exception! (Not Miro, not CORBA)
    cout << "Unknown Exception... MUST be debugged" << endl;
    //    throw;
  }

  //  reactorTask.cancel();
  reactorTask.done = true;
  reactorTask.wait();

  DBG(cout << "reactor Task ended" << endl);
  //  if (reactorTask!=NULL) delete reactorTask;
}

int
main(int argc, char *argv[])
{
  int rc = 0;

  string playerHost="localhost";
  int playerPort=PLAYER_PORTNUM;

  // Init TAO Factories
  TAO_Notify_Default_CO_Factory::init_svc();
  TAO_Notify_Default_POA_Factory::init_svc();
  TAO_Notify_Default_Collection_Factory::init_svc();
  TAO_Notify_Default_EMO_Factory::init_svc();

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Player::Parameters * simBotParameters = Player::Parameters::instance();

  PlayerClient * playerClient=NULL;

  try {
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", *robotParameters);
    config->setSection("Player");
    config->getParameters("PioneerBoard", *simBotParameters);
    delete config;
    
    DBG(cout << "Initialize server daemon." << endl);

    playerClient=new PlayerClient(playerHost.c_str(),playerPort);

    PlayerBase playerBase(argc, argv,playerClient);
 
    try {
      DBG(cout << "Loop forever handling events." << endl);
      playerBase.run(5);
      DBG(cout << "PlayerBase ended, exiting." << endl);
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
  //  catch (const Miro::CException& e) {
  //    cerr << "Miro exception: " << e << endl;
  //    rc = 1;
  //  }
  catch (const Miro::Exception& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    rc = 1;
  }

  delete playerClient;
  
  return rc;
}




