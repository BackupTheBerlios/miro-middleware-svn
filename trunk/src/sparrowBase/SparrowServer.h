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
#ifndef sparrowBase_hh
#define sparrowBase_hh

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/RangeSensorImpl.h"
#include "miro/NotifyMulticast.h"
#include "miro/OdometryImpl.h"

#include "can/CanEventHandler.h"
#include "sparrow/AliveCollector.h"

#include "sparrow/SparrowConnection.h"
#include "sparrow/SparrowConnection2003.h"
#include "sparrow/SparrowConsumer.h"
#include "sparrow/SparrowConsumer2003.h"
#include "sparrow/SparrowMotionImpl.h"
#include "sparrow/SparrowKickerImpl.h"
#include "sparrow/SparrowButtonsImpl.h"
#include "sparrow/SparrowStallImpl.h"
#include "sparrow/SparrowPanTiltImpl.h"
#include "sparrow/SparrowCMPS03Impl.h"
#include "sparrow/AliveEventHandler.h"

#include "faulMotor/FaulMotorConnection.h"
#include "faulMotor/Parameters.h"
#include "faulMotor/FaulMotorConsumer.h"
#include "faulMotor/TimerEventHandler.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <memory>

struct FaulhaberHardware
{
  // Faulhaber hardware abstraction
  ACE_Reactor * reactor;
  int timerId;
  FaulMotor::Consumer * pConsumer;
  FaulMotor::TimerEventHandler * pTimerEventHandler;
  FaulMotor::Connection connection;

  FaulhaberHardware(ACE_Reactor * _reactor,
		    Miro::OdometryImpl * _pOdometryImpl,
		    Sparrow::Connection2003 * connection2003_);
  ~FaulhaberHardware();
};

class SparrowBase
{

  // = DESCRIPTION
  //    This class starts up the BaseImpl registers it at the
  //    naming service and runs the orb

public:
  SparrowBase(Miro::Server& _server, 
	      bool startReactorTask = true);

  // Constructor.

  ~SparrowBase();
  // Destructor.


  void init(bool startReactorTask);

//   Miro::NMC::Adapter * notifyMulticast();

  // protected:
  Miro::Server& server_;

  // since TAO is using the main thread, we need our own reactor
  // actually its worse: Due to deadlocks otherwise, we need a concurrent reactor!
  Miro::ReactorTask reactorTask;

  // Notification Service

  /** Channel factory. */
  CosNotifyChannelAdmin::EventChannelFactory_ptr notifyFactory_;
  /** Channel identifer. */
  CosNotifyChannelAdmin::ChannelID id_;
  /** Initial qos specified to the factory when creating the EC. */
  CosNotification::QoSProperties initialQos_;
  /** Initial admin props specified to the factory when creating the EC. */
  CosNotification::AdminProperties initialAdmin_;
  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_ptr ec_;

  Miro::StructuredPushSupplier structuredPushSupplier_;
  Miro::StructuredPushSupplier panPushSupplier_;

  Miro::OdometryImpl odometry;
  Miro::RangeSensorImpl infrared;

  //Miro::RangeSensorImpl * infrared2;

  // Sparrow board hardware abstraction
  Sparrow::Consumer * pSparrowConsumer;
  Sparrow::Consumer2003 * pSparrowConsumer2003;
  Can::EventHandler * pCanEventHandler;
  Sparrow::Connection * sparrowConnection;
  Sparrow::Connection2003 * sparrowConnection2003;

  // Faulhaber board hardware abstraction
  FaulhaberHardware * pFaulhaber;

  // IDL interface implementations
  POA_Miro::Motion * pSparrowMotion;
  Sparrow::KickerImpl * sparrowKicker;
  Sparrow::ButtonsImpl * sparrowButtons;
  Sparrow::StallImpl *  sparrowStall;
  Sparrow::PanTiltImpl * sparrowPanTilt;
  Sparrow::CMPS03Impl * sparrowCMPS03;

  Miro::Odometry_ptr pOdometry;
  Miro::Motion_ptr pMotion;
  Miro::Kicker_ptr pKicker;
  Miro::Buttons_ptr pButtons;
  Miro::Stall_ptr pStall;
  Miro::SparrowPanTilt_ptr pPanTilt;
  Miro::RangeSensor_ptr pInfrared;
  Miro::Compass_ptr pCompass;

  /* NotifyMulticast */
//   int argc_;
//   char ** argv_;
//   Miro::NMC::Adapter *mcAdapter_;

  Sparrow::AliveCollector * aCollector;
  Sparrow::AliveEventHandler * aEventHandler_;
  ACE_Time_Value delay;
  ACE_Time_Value interval;
  int aEventHandlerId_;
};

// inline
// Miro::NMC::Adapter *
// SparrowBase::notifyMulticast() {
//   return mcAdapter_;
// }

#endif




