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
#ifndef PioneerBase_h
#define PioneerBase_h

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
#include "pioneer/CameraMessage.h"
#include "pioneer/CanonPanTiltImpl.h"
#include "pioneer/CanonCameraControlImpl.h"

#ifdef MIRO_HAS_DEPRECATED
#include "pioneer/CanonCameraImpl.h"
#endif

#include "pioneer/GripperImpl.h"
#include "pioneer/TCM2Impl.h"
#include "miro/RangeSensorImpl.h"
#include "miro/ObjectVectorImpl.h"
//#include "miro/CortexImpl.h"

#include "pioneer/Parameters.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

namespace Miro {
  namespace NMC {
    class Adapter;
  }
}

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
  //! Event channel federation for team communication.
  Miro::NMC::Adapter *mcAdapter_;
  
  Miro::StructuredPushSupplier structuredPushSupplier_;



  Miro::OdometryImpl odometry;
  Miro::BatteryImpl battery;

  Pioneer::Consumer * pPioneerConsumer;
  Psos::EventHandler * pPsosEventHandler;
  Pioneer::Connection pioneerConnection;

  Pioneer::MotionImpl motion;
  Pioneer::StallImpl  stall;
  Pioneer::TCM2Impl tcm2;
  Pioneer::CameraAnswer cameraAnswer;
  Miro::RangeSensorImpl sonar;
  Miro::RangeSensorImpl tactile;
  Miro::RangeSensorImpl infrared;
  // The implementation of these two depends on the particular camera type:
  Miro::PanTiltImpl * pPanTiltImpl;
  Miro::CameraControlImpl * pCameraControlImpl;
#ifdef MIRO_HAS_DEPRECATED
  Canon::CanonCameraImpl canonCamera;
#endif
  Miro::GripperImpl gripper;
  Miro::ObjectVectorImpl objectVector;
  //Miro::CortexImpl cortex;

  Miro::Odometry_var pOdometry;
  Miro::Motion_var pMotion;
  Miro::Stall_var pStall;
  Miro::TCM2_var pTCM2;
  Miro::RangeSensor_var pSonar;
  Miro::RangeSensor_var pTactile;
  Miro::RangeSensor_var pInfrared;
  Miro::Battery_var pBattery;
#ifdef MIRO_HAS_DEPRECATED
  Miro::CanonCamera_var pCanonCamera;
#endif
  Miro::PanTilt_var pPanTilt;
  Miro::CameraControl_var pCameraControl;
  Miro::Gripper_var pGripper;
  Miro::ObjectVector_var pObjectVector;
  //Miro::Cortex_var pCortex;

};
#endif // PioneerBase




