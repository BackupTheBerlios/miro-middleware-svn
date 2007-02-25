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
#ifndef laserServer_hh
#define laserServer_hh

#include "idl/OdometryC.h"
#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

#include "SickLaserConnection.h"
#include "SickLaserStatistic.h"
#include "SickLaserImpl.h"

namespace Laser
{
  class Parameters;
};

namespace Miro 
{
  class OdometryTracking;

  class LaserServer : public Server
  {
    typedef LaserServer Self;

    // = DESCRIPTION
    //    This class starts up the LaserImpl registers it at the
    //    naming service and runs the orb

  public:
    // Initialization and Termination methods.
    LaserServer(int argc, char *argv[]);
    // Constructor.

    ~LaserServer();
    // Destructor.

  protected:

    /** Sceduling parameters for a realtime thread */
    ACE_Sched_Params schedparams_;
    /**
     * we have put the reactor that runs in seperate task, 
     * for providing concurrency.
     */
    ReactorTask reactorTask_;

    //! CORBA reference to odometry for position stamps
    CosNotifyChannelAdmin::EventChannel_var ec_;

    StructuredPushSupplier * structuredPushSupplier_;

    OdometryTracking * odoTracking_;
    LaserImpl laser_;

    /**
     * error statistics of the comunication to the hardware
     */
    LaserStatistic * laserStatistic_;

    LaserConnection connection_;

    Miro::Laser_var pLaser;
  };
}
#endif




