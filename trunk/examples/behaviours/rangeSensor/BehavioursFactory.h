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
#ifndef RangeSensorBehavioursFactory_h
#define RangeSensorBehavioursFactory_h

#include "miro/RangeSensorC.h"
#include "miro/MotionC.h"
#include "miro/OdometryC.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/MotionArbiter.h"

#include "RangeSensorAvoid.h"
#include "RangeSensorWallFollow.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Server;
};

class RangeSensorBehavioursFactory
{
public:
  // Initialization and Termination methods.
  RangeSensorBehavioursFactory(Miro::Server& _server, ACE_Reactor& _reactor);
  // Constructor.

  ~RangeSensorBehavioursFactory();
  // Destructor.

  void open();
  void close();

private:
  Miro::Server& server_;

  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent reactor!
  ACE_Reactor& reactor_;

  // Notification Service

  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;
  Miro::StructuredPushSupplier supplier_;

  // initialize behaviours
  RangeSensorAvoid avoid1;
  RangeSensorAvoid avoid2;
  RangeSensorWallFollow wallFollow;
};
#endif




