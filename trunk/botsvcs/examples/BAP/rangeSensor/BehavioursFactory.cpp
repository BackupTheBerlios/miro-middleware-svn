// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BehavioursFactory.h"

#include "RangeSensorAvoid.h"
#include "RangeSensorWallFollow.h"

#include "miro/Server.h"

RangeSensorBehavioursFactory::RangeSensorBehavioursFactory(Miro::Server& _server) :
  // optain event channel
  ec_(_server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel")),
  supplier_(ec_.in(), _server.namingContextName, false)
{
  // initialize behaviours

  eventBehaviours_.push_back(new RangeSensorAvoid(_server, 
						  ec_.in(), 
						  "RangeSensorAvoidOne", 
						  _server.namingContextName, 
						  &supplier_));
  eventBehaviours_.push_back(new RangeSensorAvoid(_server,
						  ec_.in(),
						  "RangeSensorAvoidTwo", 
						  _server.namingContextName));
  eventBehaviours_.push_back(new RangeSensorWallFollow(_server,
						       ec_.in(),
						       "RangeSensorWallFollow", 
						       _server.namingContextName));
}

void
RangeSensorBehavioursFactory::connect()
{
  supplier_.connect();
  Super::connect();
}

void
RangeSensorBehavioursFactory::disconnect()
{
  Super::disconnect();
  supplier_.disconnect();
}
