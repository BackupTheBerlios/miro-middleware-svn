// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef B21_BehavioursFactory_h
#define B21_BehavioursFactory_h

#include "miro/Server.h"
#include "miro/RangeSensorC.h"
#include "miro/StructuredPushConsumer.h"

#include "OnButton.h"
#include "SonarAvoid.h"
#include "LaserAvoid.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declarations
class ACE_Reactor;

class B21BehavioursFactory
{
public:
  // Initialization and Termination methods.
  B21BehavioursFactory(Miro::Server& _server, ACE_Reactor& _reactor);
  // Constructor.

  ~B21BehavioursFactory();
  // Destructor.

private:
  Miro::Server& server_;
  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent reactor!
  ACE_Reactor& reactor_;

  // Notification Service

  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;

  // optain references
  Miro::RangeSensor_var sonar_;
  Miro::RangeSensor_var laser_;

  // initialize behaviours
  OnButton onButton;
  SonarAvoid sonarAvoid;
  LaserAvoid laserAvoid;
};
#endif




