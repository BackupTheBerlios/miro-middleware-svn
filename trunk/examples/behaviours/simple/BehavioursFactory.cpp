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

#include "miro/Server.h"
#include "miro/BehaviourRepository.h"
#include "miro/ArbiterRepository.h"

using CosNotifyChannelAdmin::EventChannel;

using Miro::Server;
using Miro::Motion;
using Miro::Odometry;
using Miro::MotionArbiter;
using Miro::BehaviourRepository;
using Miro::ArbiterRepository;

SimpleBehavioursFactory::SimpleBehavioursFactory(Server& _server,
						 ACE_Reactor& _reactor) :
  server_(_server),
  reactor_(_reactor),

  ec_(server_.resolveName<EventChannel>("EventChannel")),
  supplier_(ec_.in(), server_.namingContextName, false),

  // optain references
  motion_(server_.resolveName<Motion>("Motion")),
  odometry_(server_.resolveName<Odometry>("Odometry")),

  // initialize behaviours
  tactileStop(motion_.in(), ec_.in(), server_.namingContextName),
  straight(reactor_),
  wander(reactor_),
  timer(reactor_),
  moveToPoint(ec_.in(), server_.namingContextName),

  ma(motion_.in(), &supplier_)
{
  cout << "Constructing SimpleBehavioursFactory." << endl;

  BehaviourRepository * bf = BehaviourRepository::instance();
  ArbiterRepository * af = ArbiterRepository::instance();

  bf->registerBehaviour(&tactileStop);
  bf->registerBehaviour(&straight);
  bf->registerBehaviour(&wander);
  bf->registerBehaviour(&timer);
  bf->registerBehaviour(&moveToPoint);

  af->registerArbiter(&ma);
}

SimpleBehavioursFactory::~SimpleBehavioursFactory()
{
}

void
SimpleBehavioursFactory::open()
{
  supplier_.connect();

}

void
SimpleBehavioursFactory::close()
{
  supplier_.disconnect();
}

