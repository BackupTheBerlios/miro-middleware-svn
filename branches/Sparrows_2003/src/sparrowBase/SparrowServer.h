// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef sparrowBase_hh
#define sparrowBase_hh

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/RangeSensorImpl.h"
#include "miro/NotifyMulticast.h"
#include "miro/OdometryImpl.h"

#include "can/CanEventHandler.h"
#include "sparrow/AliveCollector.h"

#include "sparrow/SparrowConnection.h"
#include "sparrow/SparrowConnection2003.h"
#include "sparrow/SparrowConsumer.h"
#include "sparrow/SparrowConsumer2003.h"
#include "sparrow/SparrowMotionImpl.h"
#include "sparrow/SparrowKickerImpl.h"
#include "sparrow/SparrowButtonsImpl.h"
#include "sparrow/SparrowStallImpl.h"
#include "sparrow/SparrowPanTiltImpl.h"
#include "sparrow/AliveEventHandler.h"
#include "psos/PsosEventHandler.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"

#include "faulMotor/FaulMotorConnection.h"
#include "faulMotor/Parameters.h"
#include "faulMotor/FaulMotorConsumer.h"
#include "faulMotor/TimerEventHandler.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <memory>

struct FaulhaberHardware
{
  // Faulhaber hardware abstraction
  ACE_Reactor * reactor;
  int timerId;
  FaulMotor::Consumer * pConsumer;
  FaulMotor::TimerEventHandler * pTimerEventHandler;
  FaulMotor::Connection connection;

  FaulhaberHardware(ACE_Reactor * _reactor,
		    Miro::OdometryImpl * _pOdometryImpl);
  ~FaulhaberHardware();
};

struct PioneerHardware
{
  // Pioneer board hardware abstraction
  Pioneer::Consumer * pConsumer;
  Psos::EventHandler * pEventHandler;
  Pioneer::Connection connection;

  PioneerHardware(ACE_Reactor * _reactor,
		  Miro::RangeSensorImpl * _sonar);
  ~PioneerHardware();
};


class SparrowBase : public Miro::Server
{
  typedef Miro::Server Super;

  // = DESCRIPTION
  //    This class starts up the BaseImpl registers it at the
  //    naming service and runs the orb

public:
  // Initialization and Termination methods.
  SparrowBase(int argc, char *argv[]);

  SparrowBase(Server& _server, bool startReactorTask = true);

  // Constructor.

  ~SparrowBase();
  // Destructor.


  void init(bool startReactorTask);

  Miro::NotifyMulticast::Adapter * notifyMulticast();

  // protected:
  /** Sceduling parameters for a realtime thread */
  ACE_Sched_Params schedparams_;
  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent reactor!
  Miro::ReactorTask reactorTask;

  // Notification Service

  /** Channel factory. */
  CosNotifyChannelAdmin::EventChannelFactory_ptr notifyFactory_;
  /** Channel identifer. */
  CosNotifyChannelAdmin::ChannelID id_;
  /** Initial qos specified to the factory when creating the EC. */
  CosNotification::QoSProperties initialQos_;
  /** Initial admin props specified to the factory when creating the EC. */
  CosNotification::AdminProperties initialAdmin_;
  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_ptr ec_;

  Miro::StructuredPushSupplier structuredPushSupplier_;

  Miro::OdometryImpl odometry;
//  Miro::RangeSensorImpl * pSonar_;
  Miro::RangeSensorImpl infrared;

  // Sparrow board hardware abstraction
  Sparrow::Consumer * pSparrowConsumer;
  Sparrow::Consumer2003 * pSparrowConsumer2003;
  Can::EventHandler * pCanEventHandler;
  Sparrow::Connection * sparrowConnection;
  Sparrow::Connection2003 * sparrowConnection2003;

  // Pioneer board hardware abstraction
  PioneerHardware * pPioneer;

  // Faulhaber board hardware abstraction
  FaulhaberHardware * pFaulhaber;

  // IDL interface implementations
  POA_Miro::Motion * pSparrowMotion;
  Sparrow::KickerImpl * sparrowKicker;
  Sparrow::ButtonsImpl * sparrowButtons;
  Sparrow::StallImpl *  sparrowStall;
  Sparrow::PanTiltImpl * sparrowPanTilt;

  Miro::Odometry_ptr pOdometry;
  Miro::Motion_ptr pMotion;
  Miro::Kicker_ptr pKicker;
  Miro::Buttons_ptr pButtons;
  Miro::Stall_ptr pStall;
  Miro::SparrowPanTilt_ptr pPanTilt;
  Miro::RangeSensor_ptr pInfrared;

//  Miro::RangeSensor_ptr pSonar;

  /* NotifyMulticast */
  Miro::NotifyMulticast::Adapter *mcAdapter_;

  Sparrow::AliveCollector * aCollector;
  Sparrow::AliveEventHandler * aEventHandler;
  ACE_Time_Value delay;
  ACE_Time_Value interval;
};

inline
Miro::NotifyMulticast::Adapter *
SparrowBase::notifyMulticast() {
  return mcAdapter_;
}

#endif




