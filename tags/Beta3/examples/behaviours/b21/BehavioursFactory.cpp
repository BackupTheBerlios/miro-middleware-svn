// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BehavioursFactory.h"

#include "miro/Server.h"
#include "miro/BehaviourRepository.h"

using CosNotifyChannelAdmin::EventChannel;

using Miro::Server;
using Miro::RangeSensor;
using Miro::BehaviourRepository;
using Miro::ArbiterRepository;

B21BehavioursFactory::B21BehavioursFactory(Server& _server,
					   ACE_Reactor& _reactor) :
  server_(_server),
  reactor_(_reactor),

  ec_(server_.resolveName<EventChannel>("EventChannel")),

  // optain references
  sonar_(server_.resolveName<RangeSensor>("Sonar")),
  laser_(server_.resolveName<RangeSensor>("Laser")),

  // initialize behaviours
  onButton(ec_.in(), server_.namingContextName),
  sonarAvoid(sonar_.in(), ec_.in(), server_.namingContextName),
  laserAvoid(reactor_, laser_.in())
{
  std::cout << "Constructing B21BehavioursFactory." << std::endl;

  BehaviourRepository * bf = BehaviourRepository::instance();

  bf->registerBehaviour(&onButton);
  bf->registerBehaviour(&sonarAvoid);
  bf->registerBehaviour(&laserAvoid);
}

B21BehavioursFactory::~B21BehavioursFactory()
{
}
