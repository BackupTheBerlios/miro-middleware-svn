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
