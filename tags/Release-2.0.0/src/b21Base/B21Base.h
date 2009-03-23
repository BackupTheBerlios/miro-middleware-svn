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
#ifndef B21Base_h
#define B21Base_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/StructuredPushSupplier.h"

#include "base/BaseImpl.h"

#include "msp/MspConsumer.h"
#include "msp/MspConnection.h"
#include "msp/MspEventHandler.h"
#include "msp/Parameters.h"

#include "buttons/ButtonsConnection.h"
#include "buttons/ButtonsImpl.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

class B21Base : public Miro::Server
{
  typedef Miro::Server Super;
  typedef B21Base self;

  // = DESCRIPTION
  //    This class starts up the B21BaseImpl registers it at the
  //    naming service and runs the orb

public:
  // Initialization and Termination methods.
  B21Base(int& argc, char *argv[]);
  // Constructor.

  ~B21Base();
  // Destructor.

private:
  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent reactor!
  Miro::ReactorTask reactorTask;

  // Notification Service

  /** Channel factory. */
  CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory_;
 /** Channel identifer. */
  CosNotifyChannelAdmin::ChannelID id_;
  /** Initial qos specified to the factory when creating the EC. */
  CosNotification::QoSProperties initialQos_;
  /** Initial admin props specified to the factory when creating the EC. */
  CosNotification::AdminProperties initialAdmin_;
  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;
  
  Miro::StructuredPushSupplier structuredPushSupplier_;

  // Msp hardware abstractions

  Msp::Consumer *     pMspConsumer;  // asynchronous processing of mcp output
  Msp::EventHandler * pMspEventHandler;
  Msp::Connection     mspConnection; // encapsulating communication to hardware

  // parallel port buttons abstraction
  B21Buttons::EventHandler * pB21ButtonsEventHandler;
  B21Buttons::Connection     b21ButtonsConnection;

  // Odometry interface
  Miro::OdometryImpl odometry;
  // B21Motion interface
  Base::Impl b21Motion;
  // Msp Infrared interface
  Miro::RangeSensorImpl infrared;
  // Msp Sonar Interface
  Miro::RangeSensorImpl sonar;
  // Msp Tactile Interface
  Miro::RangeSensorImpl tactile;
  // Parallel port buttons interface
  B21Buttons::B21ButtonsImpl b21Buttons;
};
#endif




