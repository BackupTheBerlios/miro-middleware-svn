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
#ifndef SimpleBehavioursFactory_h
#define SimpleBehavioursFactory_h

#include "miro/MotionC.h"
#include "miro/OdometryC.h"
#include "miro/MotionArbiter.h"
#include "miro/StructuredPushSupplier.h"

#include "TactileStop.h"
#include "Straight.h"
#include "Wander.h"
#include "Timer.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Server;
};


class SimpleBehavioursFactory
{
public:
  // Initialization and Termination methods.
  SimpleBehavioursFactory(Miro::Server& _server, ACE_Reactor& _reactor);
  // Constructor.

  ~SimpleBehavioursFactory();
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

  // optain references
  Miro::Motion_var motion_;
  Miro::Odometry_var odometry_;

  // initialize behaviours
  TactileStop tactileStop;
  Straight straight;
  Wander wander;
  Timer timer;

  Miro::MotionArbiter ma;
};
#endif




