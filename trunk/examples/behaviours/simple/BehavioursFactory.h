// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef SimpleBehavioursFactory_h
#define SimpleBehavioursFactory_h

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


class SimpleBehavioursFactory : public Miro::BAFactory
{
  typedef Miro::BAFactory Super;

public:
  // Initialization and Termination methods.
  SimpleBehavioursFactory(Miro::Server& _server, ACE_Reactor& _reactor);
  // Constructor.

  void connect();
  void disconnect();

protected:
  // Notification Service

  /** The one channel that we create using the factory. */
  CosNotifyChannelAdmin::EventChannel_var ec_;
  Miro::StructuredPushSupplier supplier_;

  // optain references
  Miro::Motion_var motion_;
  Miro::Odometry_var odometry_;
};
#endif




