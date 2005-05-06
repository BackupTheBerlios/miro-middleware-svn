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

#include "BehavioursFactory.h"
#include "TactileStop.h"
#include "Straight.h"
#include "Wander.h"
#include "Timer.h"
#include "MoveToPoint.h"

#include "botsvcs/BAP/MotionArbiter.h"

#include "miro/Server.h"


SimpleBehavioursFactory::SimpleBehavioursFactory(Miro::Server& _server, 
						 ACE_Reactor& _reactor) :
  // optain event channel
  ec_(_server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel")),
  supplier_(ec_.in(), _server.namingContextName, false),
  // optain interface references
  motion_(_server.resolveName<Miro::Motion>("Motion")),
  odometry_(_server.resolveName<Miro::Odometry>("Odometry"))
{
  // initialize behaviours
  timedBehaviours_.push_back(new Straight(_reactor));
  timedBehaviours_.push_back(new Wander(_reactor));
  timedBehaviours_.push_back(new Timer(_reactor));

  eventBehaviours_.push_back(new TactileStop(motion_.in(), ec_.in(), _server.namingContextName));
  eventBehaviours_.push_back(new MoveToPoint(ec_.in(), _server.namingContextName));

  arbiters_.push_back(new Miro::BAP::MotionArbiter(motion_.in(), &supplier_));
}

void
SimpleBehavioursFactory::connect()
{
  supplier_.connect();
  Super::connect();
}

void
SimpleBehavioursFactory::disconnect()
{
  Super::disconnect();
  supplier_.disconnect();
}

