// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
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
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"

#include "psos/PsosEventHandler.h"
#include "pioneer/PioneerConnection.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/PioneerMotionImpl.h"
#include "pioneer/PioneerStallImpl.h"
#include "pioneer/CanonPanTiltImpl.h"
#include "pioneer/CanonCameraImpl.h"
#include "miro/RangeSensorImpl.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

class PioneerBase : public Miro::Server
{
  typedef Miro::Server Super;

  // = DESCRIPTION
  //    This class starts up the BaseImpl registers it at the
  //    naming service and runs the orb

public:
  // Initialization and Termination methods.
  PioneerBase(int argc, char *argv[]);
  // Constructor.

  ~PioneerBase();
  // Destructor.

private:
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
  Miro::BatteryImpl battery;

  Pioneer::Consumer * pPioneerConsumer;
  Psos::EventHandler * pPsosEventHandler;
  Pioneer::Connection pioneerConnection;

  Pioneer::MotionImpl motion;
  Pioneer::StallImpl  stall;
  Miro::RangeSensorImpl sonar;
  Miro::RangeSensorImpl tactile;
  Canon::CanonPanTiltImpl canonPanTilt;
  Canon::CanonCameraImpl canonCamera;

  Miro::Odometry_var pOdometry;
  Miro::Motion_var pMotion;
  Miro::Stall_var pStall;
  Miro::RangeSensor_var pSonar;
  Miro::RangeSensor_var pTactile;
  Miro::Battery_var pBattery;
  Miro::CanonPanTilt_var pCanonPanTilt;
  Miro::CanonCamera_var pCanonCamera;
};
#endif




