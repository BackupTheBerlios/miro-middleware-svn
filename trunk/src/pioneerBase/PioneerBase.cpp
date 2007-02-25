// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "PioneerBase.h"

#include "miro/NotifyMulticastAdapter.h"
#include "miro/Exception.h"
#include "miro/Configuration.h"
#include "miro/Synch.h"
#include "miro/Log.h"

#include "pioneer/Parameters.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

PioneerBase::PioneerBase(int argc, char *argv[]) :
  Super(argc, argv),
  reactorTask(this),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  mcAdapter_((Pioneer::Parameters::instance()->channelSharing)?
	     new Miro::NMC::Adapter(argc, argv, this, ec_.in(), namingContextName) :
	     NULL),
  structuredPushSupplier_(ec_.in(), namingContextName),

  odometry(&structuredPushSupplier_),
  battery(&structuredPushSupplier_),

  // Pioneer board initialization
  pPioneerConsumer(new Pioneer::Consumer(&sonar,
					 &tactile,
					 &infrared,
					 &motion,
					 &odometry,
					 &battery,
					 NULL, //stall
					 (Pioneer::Parameters::instance()->tcm2?&tcm2:NULL),
					 //only add the camera if really present
					 (Pioneer::Parameters::instance()->camera?&cameraAnswer:NULL))),
  pPsosEventHandler(new Psos::EventHandler(pPioneerConsumer, 
					   pioneerConnection,
					   Pioneer::Parameters::instance())),
  pioneerConnection(reactorTask.reactor(), pPsosEventHandler, pPioneerConsumer),

  // Service initialization
  motion(pioneerConnection),
  stall(/*pioneerConnection*/),
  tcm2(Pioneer::Parameters::instance()->tcm2Params, &structuredPushSupplier_),
  cameraAnswer(),
  sonar(Pioneer::Parameters::instance()->sonarDescription, &structuredPushSupplier_),
  tactile(Pioneer::Parameters::instance()->tactileDescription, &structuredPushSupplier_),
  infrared(Pioneer::Parameters::instance()->infraredDescription, &structuredPushSupplier_),
#ifdef MIRO_HAS_DEPRECATED
  canonCamera(pioneerConnection, static_cast<Canon::Answer *>(&cameraAnswer)),
#endif
  gripper(pioneerConnection),
  objectVector()
//       	,  cortex()
{
  pOdometry = odometry._this();
  pMotion = motion._this();
  pStall = stall._this();
  pSonar = sonar._this();
  pTactile = tactile._this();
  pInfrared = infrared._this();
  pBattery = battery._this();
  pTCM2 = tcm2._this();
  if (Pioneer::Parameters::instance()->camera) {
    if (Pioneer::Parameters::instance()->cameraParams.vendor=="canon") {
      // Canon VC series
      Canon::Answer * pAnswer = static_cast<Canon::Answer *>(&cameraAnswer);
      pCameraControlImpl=new 
	Canon::CanonCameraControlImpl(Pioneer::Parameters::instance(), 
				      pioneerConnection, 
				      pAnswer);
      pCameraControl = pCameraControlImpl->_this();
      pPanTiltImpl=new 
	Canon::CanonPanTiltImpl(pioneerConnection, 
				pAnswer,
				Pioneer::Parameters::instance()->panTiltParams, 
				Pioneer::Parameters::instance()->cameraParams.upsideDown);
      pPanTilt = pPanTiltImpl->_this();
#ifdef MIRO_HAS_DEPRECATED
      pCanonCamera = canonCamera._this();
#endif
    } // camera is canon
    else if (Pioneer::Parameters::instance()->cameraParams.vendor=="sony") {
      // Sony EVI-D30/D31 series
      pCameraControlImpl=new 
	Sony::SonyCameraControlImpl(pioneerConnection,
			*Pioneer::Parameters::instance());
      pCameraControl = pCameraControlImpl->_this();
      pPanTiltImpl=new
	Sony::SonyPanTiltImpl(pioneerConnection,
			Pioneer::Parameters::instance()->panTiltParams,
			Pioneer::Parameters::instance()->cameraParams.upsideDown);
      pPanTilt = pPanTiltImpl->_this();
    } // camera is sony
  } else {
    pCameraControlImpl=NULL;
    pPanTiltImpl=NULL;
    pCameraControl=NULL;
    pPanTilt=NULL;
  }
  pGripper = gripper._this();
  pObjectVector = objectVector._this();
 // pCortex = cortex._this();

  addToNameService(pOdometry.in(), "Odometry");
  addToNameService(pMotion.in(), "Motion");
  addToNameService(pStall.in(), "Stall");
  addToNameService(pSonar.in(), "Sonar");
  addToNameService(pTactile.in(), "Tactile");
  addToNameService(pInfrared.in(), "Infrared");
  addToNameService(pBattery.in(), "Battery");
  if (Pioneer::Parameters::instance()->tcm2)
    addToNameService(pTCM2.in(), "TCM2");
  addToNameService(ec_.in(), "EventChannel");
  addToNameService(pObjectVector.in(), "ObjectVector");
  //addToNameService(pCortex.in(), "Cortex");

  //only add the pantilt if the camera is actually present
  if (Pioneer::Parameters::instance()->camera) {
    addToNameService(pPanTilt.in(), "PanTilt");
    addToNameService(pCameraControl.in(), "CameraControl");
#ifdef MIRO_HAS_DEPRECATED
    addToNameService(pCanonCamera.in(), "Camera");
#endif
  }

  addToNameService(pGripper.in(), "Gripper");

  // start the asychronous consumer listening for the hardware
  reactorTask.open(0);
  // wait for synchronization
  pioneerConnection.waitSynch(ACE_Time_Value(10));

  if (mcAdapter_)
    mcAdapter_->init();

  // Switch TCM2 into continuous mode
  if (Pioneer::Parameters::instance()->tcm2 &&
      !(Pioneer::Parameters::instance()->tcm2Params.compassOnly))
    pioneerConnection.setTCMMode(3);

  MIRO_LOG_CTOR("PioneerBase");
}

PioneerBase::~PioneerBase()
{
  MIRO_LOG_DTOR("PioneerBase");

  // close channel sharing
  if (mcAdapter_) {
    MIRO_LOG(LL_CTOR_DTOR, "PioneerBase: Closing multicats adapter.");

    mcAdapter_->fini();
    delete mcAdapter_;
  }

  if (pCameraControlImpl)
    delete(pCameraControlImpl);
  if (pPanTiltImpl)
    delete(pPanTiltImpl);

  pioneerConnection.close();

  // Deactivate.
//  PortableServer::ObjectId_var oid =
//    poa->reference_to_id (notifyFactory_.in());
  // deactivate from the poa.
//  poa->deactivate_object (oid.in ());
  reactorTask.cancel();
  MIRO_LOG(LL_CTOR_DTOR, "PioneerBase: Reactor Task ended.");
}

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
    Pioneer::Parameters * pioneerParameters = Pioneer::Parameters::instance();

    // Config file processing
    MIRO_LOG(LL_NOTICE,"Getting config document");
    Miro::ConfigDocument * config = Miro::Configuration::document();
    config->setSection("Robot");
    config->getParameters("Miro::RobotParameters", *robotParameters);
    MIRO_LOG(LL_NOTICE,"Got Robot Parameters");
    config->setSection("ActiveMedia");
    config->getParameters("Pioneer::Parameters", *pioneerParameters);
      
    MIRO_LOG_OSTR(LL_NOTICE,
		  "  robot parameters:" << std::endl <<
		  *robotParameters << std::endl <<
		  "  pioneer parameters:" << std::endl <<
		  *pioneerParameters << std::endl
		  );
      
    MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
    PioneerBase pioneerBase(argc, argv);
    try {
      MIRO_LOG(LL_NOTICE, "Loop forever handling events.");
      pioneerBase.run(8);
      MIRO_LOG(LL_NOTICE, "pioneerBase ended, exiting.");
      rc = 0;
    }
    catch (const Miro::EOutOfBounds& e) {
      MIRO_LOG_OSTR(LL_CRITICAL,
		    "OutOfBounds exception: Wrong parameter for device initialization." << std::endl <<
		    e.what);
    }
    catch (const Miro::EDevIO& e) {
      MIRO_LOG_OSTR(LL_CRITICAL,
		    "DevIO exception: Device access failed." << std::endl <<
		    e.what);
    }
    catch (const CORBA::Exception & e) {
      MIRO_LOG_OSTR(LL_CRITICAL, "Uncaught CORBA exception: " << e);
    }
  }
  catch (const Miro::Exception& e) {
       MIRO_LOG_OSTR(LL_CRITICAL, "Miro exception: " << e);
       rc = 1;
  }

  return rc;
}




