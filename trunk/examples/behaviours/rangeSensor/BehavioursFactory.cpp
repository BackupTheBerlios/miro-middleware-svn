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

using CosNotifyChannelAdmin::EventChannel;

using Miro::Server;
using Miro::RangeSensor;
using Miro::Motion;
using Miro::Odometry;
using Miro::BehaviourRepository;

RangeSensorBehavioursFactory::RangeSensorBehavioursFactory(Server& _server,
							   ACE_Reactor& _reactor) :
  server_(_server),
  reactor_(_reactor),

  ec_(server_.resolveName<EventChannel>("EventChannel")),
  supplier_(ec_.in(), server_.namingContextName, false),

  // initialize behaviours
  avoid1(server_, ec_.in(), "RangeSensorAvoidOne", server_.namingContextName, &supplier_),
  avoid2(server_, ec_.in(), "RangeSensorAvoidTwo", server_.namingContextName),
  wallFollow(server_, ec_.in(), "RangeSensorWallFollow", server_.namingContextName)
{
  cout << "Constructing RangeSensorBehavioursFactory." << endl;

  BehaviourRepository * bf = BehaviourRepository::instance();

  bf->registerBehaviour(&avoid1);
  bf->registerBehaviour(&avoid2);
  bf->registerBehaviour(&wallFollow);
}

RangeSensorBehavioursFactory::~RangeSensorBehavioursFactory()
{
}

void
RangeSensorBehavioursFactory::open()
{
  supplier_.connect();

}

void
RangeSensorBehavioursFactory::close()
{
  supplier_.disconnect();
}
