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

#include "BehaviourEngine.h"

#include "miro/BehaviourRepository.h"
#include "miro/ArbiterRepository.h"
#include "miro/MotionArbiter.h"
#include "miro/ActionPattern.h"
#include "miro/Policy.h"

using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::InterFilterGroupOperator;
using CosNotification::QoSProperties;
using CosNotification::AdminProperties;
using CosNotifyChannelAdmin::AdminID;

using Miro::Server;
using Miro::RangeSensor;
using Miro::Motion;
using Miro::Odometry;
using Miro::MotionArbiter;
using Miro::BehaviourRepository;
using Miro::ArbiterRepository;

BehaviourEngine::BehaviourEngine(const Server& _server,
				 ACE_Reactor& _reactor) :
  Super(_server),
  reactor_(_reactor),

  ec_(resolveName<EventChannel>("EventChannel")),

  // optain references
  sonar_(resolveName<RangeSensor>("Sonar")),
  laser_(resolveName<RangeSensor>("Laser")),
  motion_(resolveName<Motion>("Motion")),
  odometry_(resolveName<Odometry>("Odometry")),

  // initialize behaviours
  onButton(ec_.in(), namingContextName),
  tactileStop(motion_.in(), ec_.in(), namingContextName),
  sonarAvoid(sonar_.in(), ec_.in(), namingContextName),
  laserAvoid(reactor_, laser_.in()),
  turnRight(reactor_,laser_.in()),
  straight(reactor_),
  wander(reactor_),

  ma(motion_.in())
{
  cout << "Constructing BehaviourEngine." << endl;

  BehaviourRepository * bf = BehaviourRepository::instance();
  ArbiterRepository * af = ArbiterRepository::instance();

  bf->registerBehaviour(&onButton);
  bf->registerBehaviour(&tactileStop);
  bf->registerBehaviour(&sonarAvoid);
  bf->registerBehaviour(&laserAvoid);
  bf->registerBehaviour(&turnRight);
  bf->registerBehaviour(&straight);
  bf->registerBehaviour(&wander);

  af->registerArbiter(&ma);
}

BehaviourEngine::~BehaviourEngine()
{
  motion_->limp();
}
