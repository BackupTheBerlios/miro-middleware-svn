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

#include "botsvcs/BAP/BAFactory.h"
#include "botsvcs/BAP/MotionArbiter.h"

#include "idl/MotionC.h"
#include "idl/OdometryC.h"
#include "miro/StructuredPushSupplier.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declarations
class ACE_Reactor;
namespace Miro
{
  class Server;
};


class RangeSensorBehavioursFactory : public Miro::BAP::BAFactory
{
  typedef Miro::BAP::BAFactory Super;

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



