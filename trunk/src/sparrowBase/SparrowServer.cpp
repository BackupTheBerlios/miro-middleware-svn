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
#include "sparrow/SparrowDevice.h"
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
using std::endl;

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
  if (params->odometryPolling) {
    timerId = reactor->schedule_timer(pTimerEventHandler, NULL, 
		                      ACE_Time_Value(2),
				      params->odometryPace);
  }
}

FaulhaberHardware::~FaulhaberHardware()
{
  if (timerId != -1)
    reactor->cancel_timer(timerId);

}


SparrowBase::SparrowBase(Miro::Server& _server, bool _startReactorTastk) :
  server_(_server),
  schedparams_(ACE_SCHED_FIFO, 10),
  reactorTask( &_server, 20, &schedparams_),

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
  mcAdapter_(( (Sparrow::Parameters::instance()->channelSharing)?
	       new Miro::NotifyMulticast::Adapter(0, NULL, &server_, ec_) :
	       NULL) ),
  delay(0,0),
  interval(0, 500000)
{
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
      new Sparrow::PanTiltImpl(sparrowConnection2003, &panPushSupplier_);
    
    pSparrowConsumer2003->
      registerInterfaces(sparrowConnection2003,
			 ( (Sparrow::Parameters::instance()->faulhaber)?
			   NULL :
			   &odometry),
			 &infrared,
			 pFaulhaber->pConsumer,
			 aCollector);
    
    pSparrowMotion = 
      static_cast<POA_Miro::Motion *>
      (new FaulMotor::MotionImpl(pFaulhaber->connection));
    
    aEventHandler = new Sparrow::AliveEventHandler(aCollector, 
						   sparrowConnection2003, 
						   &structuredPushSupplier_);
    
    reactorTask.reactor()->schedule_timer(aEventHandler, NULL, delay,interval);
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
    sparrowPanTilt = new Sparrow::PanTiltImpl(sparrowConnection, &panPushSupplier_);
    
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
  if (_startReactorTastk)
    reactorTask.open(0);

  if (mcAdapter_)
    mcAdapter_->init();
}

SparrowBase::~SparrowBase()
{
  DBG(cout << "Destructing SparrowBase." << endl);

  // close channel sharing
  if (mcAdapter_) {
    DBG(cout << "Closing multicats adapter." << endl);

    mcAdapter_->fini();
    delete mcAdapter_;
  }

  odometry.cancel();
  DBG(cout << "Odometry dispatching canceled." << endl);

  if (!Sparrow::Parameters::instance()->sparrow2003)
     infrared.cancel();
  DBG(cout << "Infrared dispatching canceled." << endl);

  DBG(cout << "Shutting down sparrow board." << endl);
  if (sparrowConnection)
    sparrowConnection->fini();
  std::cout << "success" << endl;

  std::cout << "reactor task" << endl;
  reactorTask.cancel();
  DBG(cout << "reactor Task canceled." << endl);

  DBG(cout << "removing objects from POA" << endl);

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
  oid = server_.poa->reference_to_id (notifyFactory_);
  server_.poa->deactivate_object (oid.in());

  DBG(cout << "Destructing SparrowBase members." << endl);
}
