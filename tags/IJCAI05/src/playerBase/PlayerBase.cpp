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
#include "miro/Configuration.h"

#include "Parameters.h"
#include "laser/Parameters.h"

#include "miro/OdometryTracking.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

#include <playerclient.h>

#include <ace/Arg_Shifter.h>

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

PlayerBase::PlayerBase(int argc, char *argv[],
		       PlayerClient* client, 
		       int playerId
		       ) throw (CORBA::Exception) :
  Super(argc, argv),
  reactorTask(client,playerId,&sonar,&laser,&infrared,&tactile,&odometry,&motion,&battery,&panTilt,&cameraControl,&stall),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  // Service initialization
  odometry(&structuredPushSupplier_),
  battery(),
  motion(Player::Parameters::instance()->motion),
  stall(&structuredPushSupplier_),
  sonar(Player::Parameters::instance()->sonarDescription, &structuredPushSupplier_),
  tactile(Player::Parameters::instance()->tactileDescription, &structuredPushSupplier_),
  //TODO: infrared, not sonar description!
  infrared(Player::Parameters::instance()->sonarDescription, &structuredPushSupplier_),
  laser(new Miro::OdometryTracking(ec_.in(), namingContextName),
	Laser::Parameters::instance()->laserDescription, 
    	&structuredPushSupplier_),
  panTilt(Player::Parameters::instance()->panTiltParams,Player::Parameters::instance()->cameraParams.upsideDown),
  cameraControl(Player::Parameters::instance()->cameraParams),
  playerClient(client)
{ 

  pOdometry = odometry._this();
  pMotion = motion._this();
  pStall = stall._this();
  pSonar = sonar._this();
  pTactile = tactile._this();
  pInfrared = infrared._this();
  pLaser = laser._this();
  pBattery = battery._this();
  pPanTilt = panTilt._this();
  pCameraControl = cameraControl._this();
  //  pGripper = gripper._this();

  addToNameService(pOdometry.in(), "Odometry");
  addToNameService(pMotion.in(), "Motion");
  if (reactorTask.stallBound())
    addToNameService(pStall.in(), "Stall");
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
  if (reactorTask.panTiltBound())
    addToNameService(pPanTilt.in(),"PanTilt");
  if (reactorTask.cameraControlBound())
    addToNameService(pCameraControl.in(),"CameraControl");


  //  if (reactorTask.gripperBound())
  //    addToNameService(pGripper.in(), "Gripper");

  // start the asychronous consumer listening for the hardware
  reactorTask.open(0);

  MIRO_LOG(LL_NOTICE, "PlayerBase initialized..");
}

PlayerBase::~PlayerBase()
{
  MIRO_LOG(LL_NOTICE,"Destructing PlayerBase.");

  try {
    // Deactivate.
    PortableServer::ObjectId_var oid =
      poa->reference_to_id (notifyFactory_.in());
    
    // deactivate from the poa.
    poa->deactivate_object (oid.in ());
  }
  catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR(LL_ERROR, "CORBA::Exception: " << e);
  }
  catch (const Miro::Exception & e) {
    MIRO_LOG_OSTR(LL_ERROR, "Miro::Exception: " << e);
  }
  catch (const std::string & e) {
    MIRO_LOG_OSTR(LL_ERROR, "string exception: " << e);
  }
  catch (...) {
    MIRO_LOG(LL_ERROR, "Unknown Exception... ");
  }
  
  reactorTask.done = true;
  reactorTask.wait();

  MIRO_LOG(LL_NOTICE, "reactor Task ended");
}

int parseArgs(int& argc, char* argv[], 
	      string& playerHost,
	      int& playerPort, 
	      int& playerId)
{
  ACE_Arg_Shifter get_opts(argc, argv);
  
  int rc = 0;
  
  get_opts.ignore_arg();

  while (get_opts.is_anything_left()) 
  {
    if ((get_opts.cur_arg_strncasecmp("--help")>=0)  ||
	(get_opts.cur_arg_strncasecmp("-?")>=0) ){
      cout << "usage: " << argv[0] << " [-h=hostname] [-p=port] [-i=id] [-?]" << endl
 	   << "  -h=hostname: host running Player (default "<< playerHost <<")" << endl
 	   << "  -p=port:     Player port to connect to (default " << playerPort << ")" << endl
 	   << "  -i=id:       instance id of Player devices (default " << playerId << ")" << endl
 	   << "  -?:          emit this text and stop" << endl;
      rc = 1;
      get_opts.consume_arg();
    } else if (get_opts.cur_arg_strncasecmp("-h")>=0) {
      playerHost=get_opts.get_the_parameter("-h");
      get_opts.consume_arg();
    } else if (get_opts.cur_arg_strncasecmp("-p")>=0) {
      playerPort=atoi(get_opts.get_the_parameter("-p"));
      get_opts.consume_arg();
    } else if (get_opts.cur_arg_strncasecmp("-i")>=0) {
      playerId=atoi(get_opts.get_the_parameter("-i"));
      get_opts.consume_arg();
    } else {
      get_opts.ignore_arg();
    }
  }

  return rc;
}


int
main(int argc, char *argv[])
{
  int rc = 0;

  string playerHost="localhost";
  int playerPort=PLAYER_PORTNUM;
  int playerId=0;

  // Init TAO Factories
  TAO_Notify_Default_CO_Factory::init_svc();
  TAO_Notify_Default_POA_Factory::init_svc();
  TAO_Notify_Default_Collection_Factory::init_svc();
  TAO_Notify_Default_EMO_Factory::init_svc();

  try {
    Miro::Log::init(argc, argv);
    Miro::Configuration::init(argc, argv);
  }
  catch (Miro::Exception const & e) {
    MIRO_LOG_OSTR(LL_ERROR, "Initialization of logging failed: " << std::endl << e);
    return 1;
  }

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Player::Parameters * playerParameters = Player::Parameters::instance();
  Laser::Parameters * laserParameters = Laser::Parameters::instance();
  Miro::PanParameters panParameters;
  Miro::TiltParameters tiltParameters;

  PlayerClient * playerClient=NULL;

  if ((rc=parseArgs(argc,argv,playerHost,playerPort,playerId)) != 0) 
    return rc;

  try {
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Miro::RobotParameters", *robotParameters);
    config->setSection("Player");
    config->getParameters("Player::Parameters", *playerParameters);
    config->setSection("Sick");
    config->getParameters("Laser::Parameters", *laserParameters);
    delete config;

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.");

    playerClient=new PlayerClient(playerHost.c_str(),playerPort);

    PlayerBase playerBase(argc, argv, playerClient,playerId);
 
    try {
      MIRO_LOG(LL_NOTICE, "Loop forever handling events.");
      playerBase.run(5);
      MIRO_LOG(LL_NOTICE, "PlayerBase ended, exiting.");
    }
    catch (const Miro::EOutOfBounds& e) {
      MIRO_LOG(LL_ERROR, "OutOfBounds exception: Wrong parameter for device initialization.");
    }
    catch (const Miro::EDevIO& e) {
      MIRO_LOG(LL_ERROR, "DevIO exception: Device access failed.");
    }
    catch (const CORBA::Exception & e) {
      MIRO_LOG_OSTR(LL_ERROR, "Uncaught CORBA exception: " << e);
      rc = 1;
    }
  }
  catch (const Miro::Exception& e) {
    MIRO_LOG_OSTR(LL_ERROR, "Miro exception: " << e);
    rc = 1;
  }
  catch (...) {
    MIRO_LOG(LL_ERROR, "Uncaught exception: ");
    rc = 1;
  }

  delete playerClient;
  
  return rc;
}




