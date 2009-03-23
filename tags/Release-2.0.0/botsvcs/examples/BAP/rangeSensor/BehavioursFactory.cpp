// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
