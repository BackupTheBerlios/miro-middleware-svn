// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef RangeSensorBehavioursFactory_h
#define RangeSensorBehavioursFactory_h

#include "miro/BAFactory.h"
#include "miro/MotionC.h"
#include "miro/OdometryC.h"
#include "miro/MotionArbiter.h"
#include "miro/StructuredPushSupplier.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Server;
};


class RangeSensorBehavioursFactory : public Miro::BAFactory
{
  typedef Miro::BAFactory Super;

public:
  // Initialization and Termination methods.
  RangeSensorBehavioursFactory(Miro::Server& _server);
  // Constructor.

  void connect();
  void disconnect();

protected:
  // Notification Service

  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;
  Miro::StructuredPushSupplier supplier_;
};
#endif



