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
#ifndef NotifyLogPlayer_h
#define NotifyLogPlayer_h

#include "miro/Server.h"
#include "miro/StructuredPushSupplier.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

class NotifyLogPlayer : public Miro::Server
{
  typedef Miro::Server Super;
  typedef NotifyLogPlayer self;

public:
  // Initialization and Termination methods.
  NotifyLogPlayer(int argc, char *argv[], bool colocated);
  // Constructor.

  ~NotifyLogPlayer();
  // Destructor.

  // protected:

  bool own_;
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

  Miro::StructuredPushSupplier * pPushSupplier;
};
#endif




