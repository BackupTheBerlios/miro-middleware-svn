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

