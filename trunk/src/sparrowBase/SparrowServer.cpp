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
#include "sparrow/SparrowDevice2003.h"
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
				     Miro::OdometryImpl * _pOdometryImpl,
				     Sparrow::Connection2003 * connection2003_) :
  reactor(_reactor),
  timerId(-1),
  pConsumer(new FaulMotor::Consumer(_pOdometryImpl)),
  pTimerEventHandler(new FaulMotor::TimerEventHandler(connection)),
  connection(_reactor, pConsumer, connection2003_)
{
  ACE_Time_Value tv(0,500000);

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
  structuredPushSupplier_(ec_, namingContextName),

  odometry(&structuredPushSupplier_, true, true),
  pSonar_((Sparrow::Parameters::instance()->goalie && !Sparrow::Parameters::instance()->sparrow2003)?
	  new Miro::RangeSensorImpl(Pioneer::Parameters::instance()->sonarDescription,
				    &structuredPushSupplier_, true) : NULL),
  infrared((Sparrow::Parameters::instance()->sparrow2003)?Sparrow::Parameters::instance()->infraredDescription2003:
                                                          Sparrow::Parameters::instance()->infraredDescription,
	   &structuredPushSupplier_, true),

  // Sparrow board initialization
  /*pSparrowConsumer(new Sparrow::Consumer(*sparrowConnection,
					 ( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:&sparrowStall,
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:&sparrowButtons,
					 &infrared)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer)),
  sparrowConnection(new Sparrow::Connection(reactorTask.reactor(),
		    pCanEventHandler,
		    pSparrowConsumer)),*/

  // Pioneer board initialization
  pPioneer((Sparrow::Parameters::instance()->goalie && !Sparrow::Parameters::instance()->sparrow2003)?
  	   new PioneerHardware(reactorTask.reactor(), pSonar_) : NULL),
  //pPioneer(NULL),

  // Faulhaber board initialization

  // Service initialization


  mcAdapter_((Sparrow::Parameters::instance()->channelSharing)?
             new Miro::NotifyMulticast::Adapter(argc, argv, this, ec_) :
	     0),
  delay(0,0),
  interval(0, 500000)
{

   if(Sparrow::Parameters::instance()->sparrow2003){
       std::cout << "Sparrow2003-Base-Constructor" << std::endl;
   aCollector = new Sparrow::AliveCollector();

  //infrared2 = new Miro::RangeSensorImpl(Sparrow::Parameters::instance()->infraredDescription2003,
   //                                                                &structuredPushSupplier_, false);
  pSparrowConsumer2003 = new Sparrow::Consumer2003();/*sparrowConnection,
					 ( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:sparrowStall,
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:sparrowButtons,
					 &infrared);*/
   pCanEventHandler = new Can::EventHandler(pSparrowConsumer2003);
   sparrowConnection2003 = new Sparrow::Connection2003(reactorTask.reactor(),
		    pCanEventHandler,
		    pSparrowConsumer2003);


   std::cout << "Hier 1" << flush;
   sparrowKicker = new Sparrow::KickerImpl(sparrowConnection2003);
   sparrowButtons = new Sparrow::ButtonsImpl(&structuredPushSupplier_);
   sparrowStall = new Sparrow::StallImpl(sparrowConnection2003, &structuredPushSupplier_);
   sparrowPanTilt = new Sparrow::PanTiltImpl(sparrowConnection2003);
   pFaulhaber =  new FaulhaberHardware(reactorTask.reactor(), &odometry, sparrowConnection2003);

   std::cout << "Hier 2" << flush;
   pSparrowConsumer2003->registerInterfaces(sparrowConnection2003,
					( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					   &infrared,
					   NULL,
					   pFaulhaber->pConsumer,
					   aCollector);


   pSparrowMotion = (POA_Miro::Motion *) new FaulMotor::MotionImpl(pFaulhaber->connection);

   aEventHandler = new Sparrow::AliveEventHandler(aCollector, sparrowConnection2003, &structuredPushSupplier_);
   (reactorTask.reactor())->schedule_timer(aEventHandler, NULL, delay,interval);

   }
   else{

   pSparrowConsumer = new Sparrow::Consumer();/**sparrowConnection,
					 ( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:sparrowStall,
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:sparrowButtons,
					 &infrared);*/
   pCanEventHandler = new Can::EventHandler(pSparrowConsumer);
   sparrowConnection = new Sparrow::Connection(reactorTask.reactor(),
		    pCanEventHandler,
		    pSparrowConsumer);

   sparrowKicker = new Sparrow::KickerImpl(sparrowConnection);
   sparrowButtons = new Sparrow::ButtonsImpl(&structuredPushSupplier_);
   sparrowStall = new Sparrow::StallImpl(sparrowConnection, &structuredPushSupplier_);
   sparrowPanTilt = new Sparrow::PanTiltImpl(sparrowConnection);

   pFaulhaber = ((Sparrow::Parameters::instance()->faulhaber)?
	   new FaulhaberHardware(reactorTask.reactor(), &odometry, NULL) : NULL),


   pSparrowConsumer->registerInterfaces(sparrowConnection,
					( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 sparrowStall,
					 sparrowButtons,
					 &infrared);

   pSparrowMotion = (Sparrow::Parameters::instance()->faulhaber)?
                 (POA_Miro::Motion *) new FaulMotor::MotionImpl(pFaulhaber->connection) :
		 (POA_Miro::Motion *) new Sparrow::MotionImpl(*sparrowConnection);

   }

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
  structuredPushSupplier_(ec_, namingContextName),

  odometry(&structuredPushSupplier_, true, true),
  pSonar_((Sparrow::Parameters::instance()->goalie && !Sparrow::Parameters::instance()->sparrow2003)?
	  new Miro::RangeSensorImpl(Pioneer::Parameters::instance()->sonarDescription,
				    &structuredPushSupplier_, true) : NULL),
  infrared((Sparrow::Parameters::instance()->sparrow2003)?Sparrow::Parameters::instance()->infraredDescription2003:
                                                          Sparrow::Parameters::instance()->infraredDescription,
	   &structuredPushSupplier_, true),

  // Sparrow board initialization
  /*pSparrowConsumer(new Sparrow::Consumer(*sparrowConnection,
					 ( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:&sparrowStall,
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:&sparrowButtons,
					 &infrared)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer)),
  sparrowConnection(new Sparrow::Connection(reactorTask.reactor(),
		    pCanEventHandler,
		    pSparrowConsumer)),
  */
  // Pioneer board initialization
  pPioneer((Sparrow::Parameters::instance()->goalie && !Sparrow::Parameters::instance()->sparrow2003)?
	   new PioneerHardware(reactorTask.reactor(), pSonar_) : NULL),
  //pPioneer(NULL),
  // Faulhaber board initialization

  // Service initialization


  mcAdapter_((Sparrow::Parameters::instance()->channelSharing)?
             new Miro::NotifyMulticast::Adapter(0, NULL, this, ec_) :
	     NULL),
  delay(0,0),
  interval(0, 500000)
{

   if(Sparrow::Parameters::instance()->sparrow2003){

   aCollector = new Sparrow::AliveCollector();
   //infrared2 = new Miro::RangeSensorImpl(Sparrow::Parameters::instance()->infraredDescription2003,
   //                                                                &structuredPushSupplier_, true);
   pSparrowConsumer2003 = new Sparrow::Consumer2003(); /**sparrowConnection,
					 ( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:sparrowStall,
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:sparrowButtons,
					 &infrared);*/
   pCanEventHandler = new Can::EventHandler(pSparrowConsumer2003);
   sparrowConnection2003 = new Sparrow::Connection2003(reactorTask.reactor(),
		    pCanEventHandler,
		    pSparrowConsumer2003);

   pFaulhaber = new FaulhaberHardware(reactorTask.reactor(), &odometry, sparrowConnection2003);


   sparrowKicker = new Sparrow::KickerImpl(sparrowConnection2003);
   sparrowButtons = new Sparrow::ButtonsImpl(&structuredPushSupplier_);
   sparrowStall = new Sparrow::StallImpl(sparrowConnection2003, &structuredPushSupplier_);
   sparrowPanTilt = new Sparrow::PanTiltImpl(sparrowConnection2003);






   pSparrowConsumer2003->registerInterfaces(sparrowConnection2003,
					( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 &infrared,
					 NULL,
					 pFaulhaber->pConsumer,
					 aCollector);

   pSparrowMotion = (POA_Miro::Motion *) new FaulMotor::MotionImpl(pFaulhaber->connection);

   aEventHandler = new Sparrow::AliveEventHandler(aCollector, sparrowConnection2003, &structuredPushSupplier_);


   (reactorTask.reactor())->schedule_timer(aEventHandler, NULL, delay,interval);


   }
   else{
   pSparrowConsumer = new Sparrow::Consumer(); /**sparrowConnection,
					 ( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:sparrowStall,
					 (Sparrow::Parameters::instance()->sparrow2003)?NULL:sparrowButtons,
					 &infrared);*/
   pCanEventHandler = new Can::EventHandler(pSparrowConsumer);
   sparrowConnection = new Sparrow::Connection(reactorTask.reactor(),
		    pCanEventHandler,
		    pSparrowConsumer);

   sparrowKicker = new Sparrow::KickerImpl(sparrowConnection);
   sparrowButtons = new Sparrow::ButtonsImpl(&structuredPushSupplier_);
   sparrowStall = new Sparrow::StallImpl(sparrowConnection, &structuredPushSupplier_);
   sparrowPanTilt = new Sparrow::PanTiltImpl(sparrowConnection);

   pFaulhaber = ((Sparrow::Parameters::instance()->faulhaber)?
	   new FaulhaberHardware(reactorTask.reactor(), &odometry, NULL) : NULL),


   pSparrowConsumer->registerInterfaces(sparrowConnection,
					( (Sparrow::Parameters::instance()->faulhaber)?
					   NULL :
					   &odometry),
					 sparrowStall,
					 sparrowButtons,
					 &infrared);

   pSparrowMotion = (Sparrow::Parameters::instance()->faulhaber)?
                 (POA_Miro::Motion* ) new FaulMotor::MotionImpl(pFaulhaber->connection) :
		 (POA_Miro::Motion* ) new Sparrow::MotionImpl(*sparrowConnection);



  }


  init(_startReactorTastk);

  DBG(cout << "SparrowBase initialized.." << endl);
}

void
SparrowBase::init(bool _startReactorTastk)
{

  pOdometry = odometry._this();
  pMotion = pSparrowMotion->_this();
  pInfrared = infrared._this();
  pKicker = sparrowKicker->_this();
  pPanTilt = sparrowPanTilt->_this();

  //if(!Sparrow::Parameters::instance()->sparrow2003){

    pButtons = sparrowButtons->_this();
    pStall = sparrowStall->_this();

  //}

  addToNameService(ec_, "EventChannel");
  addToNameService(pOdometry, "Odometry");
  addToNameService(pMotion, "Motion");
  addToNameService(pInfrared, "Infrared");
  addToNameService(pKicker, "Kicker");
  addToNameService(pPanTilt, "PanTilt");
  //if(!Sparrow::Parameters::instance()->sparrow2003){

    addToNameService(pButtons, "Buttons");
    addToNameService(pStall, "Stall");

  //}

  if (Sparrow::Parameters::instance()->goalie  && !Sparrow::Parameters::instance()->sparrow2003) {
    pSonar = pSonar_->_this();
    addToNameService(pSonar, "Sonar");
  }

  // start the asychronous consumer listening for the hardware
  if (_startReactorTastk)
    reactorTask.open(0);

  if (Sparrow::Parameters::instance()->channelSharing)
    mcAdapter_->init();
}

SparrowBase::~SparrowBase()
{
  DBG(cout << "Destructing SparrowBase." << endl);

  // close channel sharing
  delete mcAdapter_;

  //  sparrowConnection.readTables();
  reactorTask.cancel();
  DBG(cout << "reactor Task canceled." << endl);

  odometry.cancel();
  DBG(cout << "Odometry dispatching canceled." << endl);
  if(!Sparrow::Parameters::instance()->sparrow2003)
     infrared.cancel();

  DBG(cout << "Infrared dispatching canceled." << endl);
  if (Sparrow::Parameters::instance()->goalie  && !Sparrow::Parameters::instance()->sparrow2003) {
    pSonar_->cancel();
  }

  DBG(cout << "removing objects from POA" << endl);

  // Deactivate the interfaces.
  // we have to do this manually for none owned orbs,
  // as the class goes out of scope before
  // the orb is shut down
  PortableServer::ObjectId_var oid;
  oid =  poa->reference_to_id (pOdometry);
  poa->deactivate_object (oid.in());
  oid =  poa->reference_to_id (pMotion);
  poa->deactivate_object (oid.in());
  oid =  poa->reference_to_id (pInfrared);
  poa->deactivate_object (oid.in());
  oid =  poa->reference_to_id (pKicker);
   poa->deactivate_object (oid.in());
  oid =  poa->reference_to_id (pPanTilt);
  poa->deactivate_object (oid.in());
  //if(!Sparrow::Parameters::instance()->sparrow2003){

    oid =  poa->reference_to_id (pButtons);
    poa->deactivate_object (oid.in());
    oid = poa->reference_to_id ( pStall);
    poa->deactivate_object (oid.in());

  //}
  if (Sparrow::Parameters::instance()->goalie  && !Sparrow::Parameters::instance()->sparrow2003) {
    oid = poa->reference_to_id (pSonar);
    poa->deactivate_object(oid.in());
  }
//  cout << "." << flush;
//  oid = poa->reference_to_id (notifyFactory_);
//  poa->deactivate_object (oid.in());

  DBG(cout << "Destructing SparrowBase members." << endl);
}
