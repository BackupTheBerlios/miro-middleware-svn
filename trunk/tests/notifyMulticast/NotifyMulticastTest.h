// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef NotifyMulticastTest_h
#define NotifyMulticastTest_h

#include "miro/Server.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/NotifyMulticastAdapter.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <ace/Reactor.h>

class Consumer;

class StatisticHandler;

class NotifyMulticastTest : public Miro::Server
{
  typedef Miro::Server Super;
  typedef NotifyMulticastTest self;

public:
  // Initialization and Termination methods.
  NotifyMulticastTest(int argc, char *argv[], bool colocated);
  // Constructor.

  ~NotifyMulticastTest();
  // Destructor.

  // protected:

  bool colocated_;

  // Notification Service

  /** Channel factory. */
  CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory_;
  /** Channel identifer. */
  CosNotifyChannelAdmin::ChannelID id_;
  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;
  /** The group operator between admin-proxy's. */
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;
  /** Initial qos specified to the factory when creating the EC. */
  CosNotification::QoSProperties initialQos_;
  /** Initial admin props specified to the factory when creating the EC. */
  CosNotification::AdminProperties initialAdmin_;

  Consumer * consumer;

  Miro::NMC::Adapter *nmcAdapter;
  
  CosNotification::EventTypeSeq ets;

  ACE_Reactor *reactor_;

  StatisticHandler *sh_;
  int shId_;

  ACE_Time_Value shTime_;
};
#endif




