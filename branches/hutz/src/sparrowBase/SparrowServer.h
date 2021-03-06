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
#ifndef sparrowBase_hh
#define sparrowBase_hh

/* needed for QT/ACE compatibility */
#define QT_GENUINE_STR

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/RangeSensorImpl.h"
#include "miro/NotifyMulticast.h"
#include "miro/OdometryImpl.h"

#include "can/CanEventHandler.h"
#include "sparrow/SparrowConnection.h"
#include "sparrow/SparrowConsumer.h"
#include "sparrow/SparrowMotionImpl.h"
#include "sparrow/SparrowKickerImpl.h"
#include "sparrow/SparrowButtonsImpl.h"
#include "sparrow/SparrowStallImpl.h"
#include "sparrow/SparrowPanTiltImpl.h"

#include "psos/PsosEventHandler.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

class SparrowBase : public Miro::Server
{
  typedef Miro::Server Super;

  // = DESCRIPTION
  //    This class starts up the BaseImpl registers it at the
  //    naming service and runs the orb

public:
  // Initialization and Termination methods.
  SparrowBase(int argc, char *argv[],
	      const Miro::RobotParameters& _robotParameters,
  	      const Pioneer::Parameters& _pioneerParameters);

  SparrowBase(Server& _server,
	      const Pioneer::Parameters& _pioneerParameters);

  // Constructor.

  ~SparrowBase();
  // Destructor.


  void init();

  Miro::NotifyMulticast::Adapter * notifyMulticast();

  // protected:
  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent reactor!
  Miro::ReactorTask reactorTask;

  // Notification Service

  /** Channel factory. */
  CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory_;
 /** Channel identifer. */
  CosNotifyChannelAdmin::ChannelID id_;
  /** Initial qos specified to the factory when creating the EC. */
  CosNotification::QoSProperties initialQos_;
  /** Initial admin props specified to the factory when creating the EC. */
  CosNotification::AdminProperties initialAdmin_;
  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;
  
  Miro::StructuredPushSupplier structuredPushSupplier_;

  Miro::OdometryImpl odometry;
  Sparrow::MotionImpl sparrowMotion;

  // Sparrow board hardware abstraction
  Sparrow::Consumer * pSparrowConsumer;
  Can::EventHandler * pCanEventHandler;
  Sparrow::Connection sparrowConnection;

  // Pioneer board hardware abstraction
  Pioneer::Consumer * pPioneerConsumer;
  Psos::EventHandler * pPsosEventHandler;
  Pioneer::Connection pioneerConnection;

  // IDL interface implementations
  Sparrow::KickerImpl  sparrowKicker;
  Sparrow::ButtonsImpl sparrowButtons;
  Sparrow::StallImpl   sparrowStall;
  Sparrow::PanTiltImpl sparrowPanTilt;

  Miro::RangeSensorImpl sonar;
  Miro::RangeSensorImpl infrared;

  Miro::Odometry_var pOdometry;
  Miro::Motion_var pMotion;
  Miro::Kicker_var pKicker;
  Miro::Buttons_var pButtons;
  Miro::Stall_var pStall;
  Miro::SparrowPanTilt_var pPanTilt;
  Miro::RangeSensor_var pInfrared;
  
  Miro::RangeSensor_var pSonar;

  /* NotifyMulticast */
  Miro::NotifyMulticast::Adapter *mcAdapter_;
};

inline 
Miro::NotifyMulticast::Adapter *
SparrowBase::notifyMulticast() {
  return mcAdapter_;
}
  
#endif




