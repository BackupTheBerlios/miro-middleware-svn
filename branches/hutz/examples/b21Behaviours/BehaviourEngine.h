// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef BehaviourEngine_h
#define BehaviourEngine_h

#include "miro/Server.h"
#include "miro/RangeSensorC.h"
#include "miro/MotionC.h"
#include "miro/OdometryC.h"
#include "miro/StructuredPushConsumer.h"
#include "miro/MotionArbiter.h"

#include "OnButton.h"
#include "TactileStop.h"
#include "SonarAvoid.h"
#include "LaserAvoid.h"
#include "TurnRight.h"
#include "Straight.h"
#include "Wander.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declarations
class ACE_Reactor;

class BehaviourEngine : public Miro::Server
{
  typedef Miro::Server Super;
  typedef BehaviourEngine Self;

public:
  // Initialization and Termination methods.
  BehaviourEngine(const Miro::Server& _server, ACE_Reactor& _reactor);
  // Constructor.

  ~BehaviourEngine();
  // Destructor.

private:
  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent reactor!
  ACE_Reactor& reactor_;

  // Notification Service

  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;

  // optain references
  Miro::RangeSensor_var sonar_;
  Miro::RangeSensor_var laser_;
  Miro::Motion_var motion_;
  Miro::Odometry_var odometry_;

  // initialize behaviours
  OnButton onButton;
  TactileStop tactileStop;
  SonarAvoid sonarAvoid;
  LaserAvoid laserAvoid;
  TurnRight turnRight;
  Straight straight;
  Wander wander;

  Miro::MotionArbiter ma;
};
#endif




