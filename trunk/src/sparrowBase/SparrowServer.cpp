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
#include "faulMotor/FaulMotorMotionImpl.h"

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

FaulhaberHardware::FaulhaberHardware(ACE_Reactor * _reactor,
				     Miro::OdometryImpl * _pOdometryImpl) :
  reactor(_reactor),
  timerId(-1),
  pConsumer(new FaulMotor::Consumer(_pOdometryImpl)),
  pTimerEventHandler(new FaulMotor::TimerEventHandler(connection)),
  connection(_reactor, pConsumer)
{
  ACE_Time_Value tv(0,50000);

  timerId = reactor->schedule_timer(pTimerEventHandler, NULL, tv ,tv);
}

FaulhaberHardware::~FaulhaberHardware()
{
  reactor->cancel_timer(timerId);
 
}

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
  schedparams_(ACE_SCHED_FIFO, 10),
  reactorTask( this, 20, &schedparams_),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  odometry(&structuredPushSupplier_, true, true),
  pSonar_((Sparrow::Parameters::instance()->goalie)? 
	  new Miro::RangeSensorImpl(Pioneer::Parameters::instance()->sonarDescription, 
				    &structuredPushSupplier_, true) : NULL),
  infrared(Sparrow::Parameters::instance()->infraredDescription,
	   &structuredPushSupplier_, true),

  // Sparrow board initialization
  pSparrowConsumer(new Sparrow::Consumer(sparrowConnection,
					 ( (Sparrow::Parameters::instance()->faulhaber)? 
					   NULL :
					   &odometry),
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

  // Faulhaber board initialization
  pFaulhaber((Sparrow::Parameters::instance()->faulhaber)? 
	   new FaulhaberHardware(reactorTask.reactor(), &odometry) : NULL),

  // Service initialization
  pSparrowMotion((Sparrow::Parameters::instance()->faulhaber)?
                 (POA_Miro::Motion *) new FaulMotor::MotionImpl(pFaulhaber->connection, 
								*pFaulhaber->pConsumer) :
		 (POA_Miro::Motion *) new Sparrow::MotionImpl(sparrowConnection, 
							      *pSparrowConsumer)),
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
  schedparams_(ACE_SCHED_FIFO, 10),
  reactorTask( &_server, 20, &schedparams_),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_.in(), namingContextName),

  odometry(&structuredPushSupplier_, true, true),
  pSonar_((Sparrow::Parameters::instance()->goalie)? 
	  new Miro::RangeSensorImpl(Pioneer::Parameters::instance()->sonarDescription, 
				    &structuredPushSupplier_, true) : NULL),
  infrared(Sparrow::Parameters::instance()->infraredDescription,
	   &structuredPushSupplier_, true),

  // Sparrow board initialization
  pSparrowConsumer(new Sparrow::Consumer(sparrowConnection, 
					 ( (Sparrow::Parameters::instance()->faulhaber)? 
					   NULL :
					   &odometry),
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

  // Faulhaber board initialization
  pFaulhaber((Sparrow::Parameters::instance()->faulhaber)? 
	   new FaulhaberHardware(reactorTask.reactor(), &odometry) : NULL),

  // Service initialization
  pSparrowMotion((Sparrow::Parameters::instance()->faulhaber)?
                 (POA_Miro::Motion *) new FaulMotor::MotionImpl(pFaulhaber->connection,
								*pFaulhaber->pConsumer) :
		 (POA_Miro::Motion *) new Sparrow::MotionImpl(sparrowConnection, 
							      *pSparrowConsumer)),
  sparrowKicker(sparrowConnection),
  sparrowButtons(*pSparrowConsumer, &structuredPushSupplier_),
  sparrowStall(sparrowConnection, &structuredPushSupplier_),
  sparrowPanTilt(sparrowConnection),

  mcAdapter_((Sparrow::Parameters::instance()->channelSharing)?
             new Miro::NotifyMulticast::Adapter(0, NULL, this, ec_.in()) :
	     NULL)
{
  init(_startReactorTastk);

  DBG(cout << "SparrowBase initialized.." << endl);
}

void
SparrowBase::init(bool _startReactorTastk)
{
  pOdometry = odometry._this();
  pMotion = pSparrowMotion->_this();
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

  if (pSonar_.get() != NULL) {
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

  // close channel sharing
  delete mcAdapter_;

  //  sparrowConnection.readTables();
  reactorTask.cancel();

  odometry.cancel();
  infrared.cancel();
  if (pSonar_.get() != NULL) {
    pSonar_->cancel();
  }

  // Deactivate.
  PortableServer::ObjectId_var oid =
    poa->reference_to_id (notifyFactory_.in());

  // deactivate from the poa.
  poa->deactivate_object (oid.in ());


  DBG(cout << "reactor Task ended" << endl);
}
