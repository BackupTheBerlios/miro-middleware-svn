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

#include <tao/ORB_Core.h>

#include "sparrow/SparrowParameters.h"
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

SparrowBase::SparrowBase(int argc, char *argv[],
			 const Miro::RobotParameters& _robotParameters,
			 const Pioneer::Parameters& _pioneerParms) :
  Super(argc, argv, _robotParameters),
  reactorTask(this),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in())),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  odometry(&structuredPushSupplier_),
  sparrowMotion(sparrowConnection, *pSparrowConsumer),

  // Sparrow board initialization
  pSparrowConsumer(new Sparrow::Consumer(sparrowConnection, 
					 &odometry,
					 &sparrowStall,
					 &sparrowButtons,
					 &infrared)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer)),
  sparrowConnection(reactorTask.reactor(),
		    pCanEventHandler, 
		    pSparrowConsumer),

  // Pioneer board initialization
  pPioneerConsumer(new Pioneer::Consumer(&sonar)),
  pPsosEventHandler(new Psos::EventHandler(pPioneerConsumer, pioneerConnection)),
  pioneerConnection(reactorTask.reactor(), 
		    pPsosEventHandler, pPioneerConsumer, _pioneerParms),

  // Service initialization
  sparrowKicker(sparrowConnection),
  sparrowButtons(*pSparrowConsumer, &structuredPushSupplier_),
  sparrowStall(sparrowConnection, &structuredPushSupplier_),
  sparrowPanTilt(sparrowConnection),

  sonar(_pioneerParms.sonarDescription, &structuredPushSupplier_),
  infrared(Sparrow::Parameters::instance()->infraredDescription, 
           &structuredPushSupplier_),
  mcAdapter_((Sparrow::Parameters::instance()->channelSharing)?
             new Miro::NotifyMulticast::Adapter(argc, argv, this, ec_.in()) :
	     0)
{

  init();

  DBG(cout << "SparrowBase initialized.." << endl);
}

SparrowBase::SparrowBase(Server& _server,
			 const Pioneer::Parameters& _pioneerParms) :
  Super(_server),
  reactorTask(this),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in())),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  odometry(&structuredPushSupplier_),
  sparrowMotion(sparrowConnection, *pSparrowConsumer),

  // Sparrow board initialization
  pSparrowConsumer(new Sparrow::Consumer(sparrowConnection, 
					 &odometry,
					 &sparrowStall,
					 &sparrowButtons,
					 &infrared)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer)),
  sparrowConnection(reactorTask.reactor(),
		    pCanEventHandler, 
		    pSparrowConsumer),

  // Pioneer board initialization
  pPioneerConsumer(new Pioneer::Consumer(&sonar)),
  pPsosEventHandler(new Psos::EventHandler(pPioneerConsumer, pioneerConnection)),
  pioneerConnection(reactorTask.reactor(), pPsosEventHandler, pPioneerConsumer, _pioneerParms),

  // Service initialization
  sparrowKicker(sparrowConnection),
  sparrowButtons(*pSparrowConsumer, &structuredPushSupplier_),
  sparrowStall(sparrowConnection, &structuredPushSupplier_),
  sparrowPanTilt(sparrowConnection),

  sonar(_pioneerParms.sonarDescription, &structuredPushSupplier_),
  infrared(Sparrow::Parameters::instance()->infraredDescription,
	   &structuredPushSupplier_),
  mcAdapter_((Sparrow::Parameters::instance()->channelSharing)?
             new Miro::NotifyMulticast::Adapter(0, NULL, this, ec_.in()) :
	     0)
{
  init();

  DBG(cout << "SparrowBase initialized.." << endl);
}

void
SparrowBase::init()
{
  pOdometry = odometry._this();
  pMotion = sparrowMotion._this();
  pKicker = sparrowKicker._this();
  pButtons = sparrowButtons._this();
  pStall = sparrowStall._this();
  pPanTilt = sparrowPanTilt._this();
  pInfrared = infrared._this();
  
  pSonar = sonar._this();

  addToNameService(ec_.in(), "EventChannel");
  addToNameService(pOdometry.in(), "Odometry");
  addToNameService(pMotion.in(), "Motion");
  addToNameService(pKicker.in(), "Kicker");
  addToNameService(pButtons.in(), "Buttons");
  addToNameService(pStall.in(), "Stall");
  addToNameService(pSonar.in(), "Sonar");
  addToNameService(pPanTilt.in(), "PanTilt");
  addToNameService(pInfrared.in(), "Infrared");

  // start the asychronous consumer listening for the hardware
  reactorTask.open(0);
}

SparrowBase::~SparrowBase()
{
  DBG(cout << "Destructing SparrowBase." << endl);

  // Deactivate.
  PortableServer::ObjectId_var oid =
    poa->reference_to_id (notifyFactory_.in());

  // deactivate from the poa.
  poa->deactivate_object (oid.in ());


  sparrowConnection.readTables();
  reactorTask.cancel();
  DBG(cout << "reactor Task ended" << endl);
}
