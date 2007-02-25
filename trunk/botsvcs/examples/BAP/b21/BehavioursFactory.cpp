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

#include "OnButton.h"
#include "SonarAvoid.h"
#include "LaserAvoid.h"

#include "miro/Server.h"

B21BehavioursFactory::B21BehavioursFactory(Miro::Server& _server,
					   ACE_Reactor& _reactor)
{
  std::cout << "Constructing B21BehavioursFactory." << std::endl;

  // optain references
  CosNotifyChannelAdmin::EventChannel_var ec =
    _server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel");
  Miro::RangeSensor_var sonar =
    _server.resolveName<Miro::RangeSensor>("Sonar");
  Miro::RangeSensor_var laser =
    _server.resolveName<Miro::RangeSensor>("Laser");


  // initialize behaviours
  eventBehaviours_.push_back(new OnButton(ec.in(), 
					  _server.namingContextName));
  eventBehaviours_.push_back(new SonarAvoid(sonar.in(),
					    ec.in(),
					    _server.namingContextName));
  timedBehaviours_.push_back(new LaserAvoid(_reactor, 
					    laser.in()));
}
