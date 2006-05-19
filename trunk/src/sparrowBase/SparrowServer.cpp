// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "SparrowServer.h"

#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Log.h"

#include <tao/ORB_Core.h>

#include "sparrow/Parameters.h"
#include "sparrow/SparrowDevice.h"
#include "faulMotor/FaulMotorMotionImpl.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <orbsvcs/Notify/Notify_Default_CO_Factory.h>
#include <orbsvcs/Notify/Notify_Default_POA_Factory.h>
#include <orbsvcs/Notify/Notify_Default_Collection_Factory.h>
#include <orbsvcs/Notify/Notify_Default_EMO_Factory.h>

FaulhaberHardware::FaulhaberHardware(ACE_Reactor * _reactor,
				     Miro::OdometryImpl * _pOdometryImpl,
				     Sparrow::Connection2003 * connection2003_) :
  reactor(_reactor),
  timerId(-1),
  pConsumer(new FaulMotor::Consumer(_pOdometryImpl)),
  pTimerEventHandler(new FaulMotor::TimerEventHandler(connection)),
  connection(_reactor, pConsumer, connection2003_)
{
  FaulMotor::Parameters * params = 
    FaulMotor::Parameters::instance();
  timerId = reactor->schedule_timer(pTimerEventHandler, NULL, 
				    ACE_Time_Value(2),
				    params->odometryPace);
}

FaulhaberHardware::~FaulhaberHardware()
{
  if (timerId != -1)
    reactor->cancel_timer(timerId);
}


SparrowBase::SparrowBase(Miro::Server& _server, bool _startReactorTask) :
  server_(_server),
  reactorTask( &_server, 20, &Sparrow::Parameters::instance()->scheduling),

  // Notification Channel
  notifyFactory_(TAO_Notify_EventChannelFactory_i::
		 create(server_.poa.in() ACE_ENV_ARG_PARAMETER)),
  id_(),
  ec_(notifyFactory_->create_channel(initialQos_, initialAdmin_, id_)),
  structuredPushSupplier_(ec_, server_.namingContextName),
  panPushSupplier_(ec_, server_.namingContextName),

  odometry(&structuredPushSupplier_, true, true),
  infrared( ( (Sparrow::Parameters::instance()->sparrow2003)?
	      Sparrow::Parameters::instance()->infraredDescription2003:
	      Sparrow::Parameters::instance()->infraredDescription ),
	   &structuredPushSupplier_, true),

  // Sparrow board initialization
  sparrowConnection(NULL),

  // Faulhaber board initialization
  pFaulhaber(NULL),

  // notify multicast iniitialization
//   argc_(0),
//   argv_(NULL),
//   mcAdapter_(( (Sparrow::Parameters::instance()->channelSharing)?
// 	       new Miro::NMC::Adapter(argc_, argv_, 
// 				      &server_, ec_, server_.namingContextName) :
// 	       NULL) ),
  aEventHandler_(NULL),
  delay(0,0),
  interval(0, 500000),
  aEventHandlerId_(-1)
{
  MIRO_LOG_CTOR("SparrowBase");
  
  if(Sparrow::Parameters::instance()->sparrow2003) {
    
    aCollector = new Sparrow::AliveCollector();
    pSparrowConsumer2003 = new Sparrow::Consumer2003();
    pCanEventHandler = 
      new Can::EventHandler(pSparrowConsumer2003, Sparrow::Parameters::instance());
    sparrowConnection2003 = new Sparrow::Connection2003(reactorTask.reactor(),
							pCanEventHandler,
							pSparrowConsumer2003);
    pFaulhaber = new FaulhaberHardware(reactorTask.reactor(),
				       &odometry, 
				       sparrowConnection2003);
    sparrowKicker = new Sparrow::KickerImpl(sparrowConnection2003);
    sparrowButtons = new Sparrow::ButtonsImpl(&structuredPushSupplier_);
    sparrowStall = 
      new Sparrow::StallImpl(sparrowConnection2003, &structuredPushSupplier_);
    sparrowPanTilt = 
      new Sparrow::PanTiltImpl(sparrowConnection2003, 
			       &panPushSupplier_);
    
    pSparrowConsumer2003->
      registerInterfaces(sparrowConnection2003,
			 ( (Sparrow::Parameters::instance()->faulhaber)?
			   NULL :
			   &odometry),
			 &infrared,
			 pFaulhaber->pConsumer,
			 sparrowPanTilt,
			 aCollector);
    
    pSparrowMotion = 
      static_cast<POA_Miro::Motion *>
      (new FaulMotor::MotionImpl(pFaulhaber->connection));
    
    aEventHandler_ = new Sparrow::AliveEventHandler(aCollector, 
						   sparrowConnection2003, 
						   &structuredPushSupplier_);
    
    aEventHandlerId_ = reactorTask.reactor()->schedule_timer(aEventHandler_, NULL, delay,interval);
  }
  else {
    pSparrowConsumer = new Sparrow::Consumer();
    pCanEventHandler = 
      new Can::EventHandler(pSparrowConsumer, Sparrow::Parameters::instance());
    sparrowConnection = new Sparrow::Connection(reactorTask.reactor(),
						pCanEventHandler,
						pSparrowConsumer);
    sparrowKicker = new Sparrow::KickerImpl(sparrowConnection);
    sparrowButtons = new Sparrow::ButtonsImpl(&structuredPushSupplier_);
    sparrowStall = new Sparrow::StallImpl(sparrowConnection, &structuredPushSupplier_);
    sparrowPanTilt = new Sparrow::PanTiltImpl(sparrowConnection, 
					      &panPushSupplier_);
    
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
      static_cast<POA_Miro::Motion * >
      (new FaulMotor::MotionImpl(pFaulhaber->connection)) :
      static_cast<POA_Miro::Motion * >
      (new Sparrow::MotionImpl(*sparrowConnection));
  }

  init(_startReactorTask);
}

void
SparrowBase::init(bool _startReactorTask)
{
  pOdometry = odometry._this();
  pMotion = pSparrowMotion->_this();
  pInfrared = infrared._this();
  pKicker = sparrowKicker->_this();
  pPanTilt = sparrowPanTilt->_this();

  server_.addToNameService(ec_, "EventChannel");
  server_.addToNameService(pOdometry, "Odometry");
  server_.addToNameService(pMotion, "Motion");
  server_.addToNameService(pInfrared, "Infrared");
  server_.addToNameService(pKicker, "Kicker");
  server_.addToNameService(pPanTilt, "PanTilt");
  //if(!Sparrow::Parameters::instance()->sparrow2003){

  pButtons = sparrowButtons->_this();
  pStall = sparrowStall->_this();
  server_.addToNameService(pButtons, "Buttons");
  server_.addToNameService(pStall, "Stall");

  //}

  // start the asychronous consumer listening for the hardware
  if (_startReactorTask)
    reactorTask.open(0);

//   if (mcAdapter_)
//     mcAdapter_->init();

  MIRO_LOG(LL_CTOR_DTOR, "SparrowBase initialized.");
}

SparrowBase::~SparrowBase()
{
  MIRO_LOG_DTOR("SparrowBase");

  if (aEventHandlerId_ != -1)
    reactorTask.reactor()->cancel_timer(aEventHandlerId_);
  
  delete aEventHandler_;
  
  // close channel sharing
//   if (mcAdapter_) {
//     MIRO_LOG(LL_CTOR_DTOR, "SparrowBase: Closing multicats adapter.");

//     mcAdapter_->fini();
//     delete mcAdapter_;
//   }

  odometry.cancel();
    MIRO_LOG(LL_CTOR_DTOR, "SparrowBase: Odometry dispatching canceled.");

  infrared.cancel();
  MIRO_LOG(LL_CTOR_DTOR, "SparrowBase: Infrared dispatching canceled.");

  MIRO_LOG(LL_CTOR_DTOR, "SparrowBase: Shutting down sparrow board.");
  if (sparrowConnection)
    sparrowConnection->fini();

  reactorTask.cancel();
  MIRO_LOG(LL_CTOR_DTOR, "SparrowBase: Reactor Task canceled.");


  delete pFaulhaber;
  delete sparrowConnection;
  delete sparrowConnection2003;

  MIRO_LOG(LL_CTOR_DTOR, "SparrowBase: removing objects from POA.");

  // Deactivate the interfaces.
  // we have to do this manually for none owned orbs,
  // as the class goes out of scope before
  // the orb is shut down
  PortableServer::ObjectId_var oid;
  oid =  server_.poa->reference_to_id (pOdometry);
  server_.poa->deactivate_object (oid.in());

  oid =  server_.poa->reference_to_id (pMotion);
  server_.poa->deactivate_object (oid.in());
  oid =  server_.poa->reference_to_id (pInfrared);
  server_.poa->deactivate_object (oid.in());
  oid =  server_.poa->reference_to_id (pKicker);
   server_.poa->deactivate_object (oid.in());
  oid =  server_.poa->reference_to_id (pPanTilt);
  server_.poa->deactivate_object (oid.in());
  //if(!Sparrow::Parameters::instance()->sparrow2003){

    oid =  server_.poa->reference_to_id (pButtons);
    server_.poa->deactivate_object (oid.in());
    oid = server_.poa->reference_to_id ( pStall);
    server_.poa->deactivate_object (oid.in());

  //}
//  oid = server_.poa->reference_to_id (notifyFactory_);
//  server_.poa->deactivate_object (oid.in());

    MIRO_LOG(LL_CTOR_DTOR, "Destructing SparrowBase members." );
}
