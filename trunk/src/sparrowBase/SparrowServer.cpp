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

#include <tao/ORB_Core.h>

#include "sparrow/Parameters.h"
#include "pioneer/Parameters.h"

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

PioneerHardware::PioneerHardware(ACE_Reactor * _reactor,
				 Miro::RangeSensorImpl * _sonar) :
  pConsumer(new Pioneer::Consumer(_sonar)),
  pEventHandler(new Psos::EventHandler(pConsumer, connection)),
  connection(_reactor, pEventHandler, pConsumer)
{
}

PioneerHardware::~PioneerHardware()
{
  connection.close();
}

SparrowBase::SparrowBase(int argc, char *argv[]) :
  Super(argc, argv),
  reactorTask(this),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  odometry(&structuredPushSupplier_),
  pSonar_((Sparrow::Parameters::instance()->goalie)? 
	  new Miro::RangeSensorImpl(Pioneer::Parameters::instance()->sonarDescription, 
				    &structuredPushSupplier_) : NULL),
  infrared(Sparrow::Parameters::instance()->infraredDescription,
	   &structuredPushSupplier_),
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
  pPioneer((Sparrow::Parameters::instance()->goalie)? 
	   new PioneerHardware(reactorTask.reactor(), pSonar_.get()) : NULL),

  // Service initialization
  sparrowKicker(sparrowConnection),
  sparrowButtons(*pSparrowConsumer, &structuredPushSupplier_),
  sparrowStall(sparrowConnection, &structuredPushSupplier_),
  sparrowPanTilt(sparrowConnection),

  mcAdapter_((Sparrow::Parameters::instance()->channelSharing)?
             new Miro::NotifyMulticast::Adapter(argc, argv, this, ec_.in()) :
	     0)
{

  init(true);

  DBG(cout << "SparrowBase initialized.." << endl);
}

SparrowBase::SparrowBase(Server& _server, bool _startReactorTastk) :
  Super(_server),
  reactorTask(&_server),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  odometry(&structuredPushSupplier_),
  pSonar_((Sparrow::Parameters::instance()->goalie)? 
	  new Miro::RangeSensorImpl(Pioneer::Parameters::instance()->sonarDescription, 
				    &structuredPushSupplier_) : NULL),
  infrared(Sparrow::Parameters::instance()->infraredDescription,
	   &structuredPushSupplier_),
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
  pPioneer((Sparrow::Parameters::instance()->goalie)? 
	   new PioneerHardware(reactorTask.reactor(), pSonar_.get()) : NULL),

  // Service initialization
  sparrowKicker(sparrowConnection),
  sparrowButtons(*pSparrowConsumer, &structuredPushSupplier_),
  sparrowStall(sparrowConnection, &structuredPushSupplier_),
  sparrowPanTilt(sparrowConnection),

  mcAdapter_((Sparrow::Parameters::instance()->channelSharing)?
             new Miro::NotifyMulticast::Adapter(0, NULL, this, ec_.in()) :
	     0)
{
  init(_startReactorTastk);

  DBG(cout << "SparrowBase initialized.." << endl);
}

void
SparrowBase::init(bool _startReactorTastk)
{
  pOdometry = odometry._this();
  pMotion = sparrowMotion._this();
  pKicker = sparrowKicker._this();
  pButtons = sparrowButtons._this();
  pStall = sparrowStall._this();
  pPanTilt = sparrowPanTilt._this();
  pInfrared = infrared._this();
  
  addToNameService(ec_.in(), "EventChannel");
  addToNameService(pOdometry.in(), "Odometry");
  addToNameService(pMotion.in(), "Motion");
  addToNameService(pKicker.in(), "Kicker");
  addToNameService(pButtons.in(), "Buttons");
  addToNameService(pStall.in(), "Stall");
  addToNameService(pPanTilt.in(), "PanTilt");
  addToNameService(pInfrared.in(), "Infrared");

  if (pSonar_.get() == NULL) {
    pSonar = pSonar_->_this();
    addToNameService(pSonar.in(), "Sonar");
  }

  // start the asychronous consumer listening for the hardware
  if (_startReactorTastk)
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


  //  sparrowConnection.readTables();
  reactorTask.cancel();

  DBG(cout << "reactor Task ended" << endl);
}
