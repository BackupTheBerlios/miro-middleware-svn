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



