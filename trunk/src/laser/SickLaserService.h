// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef laserServer_hh
#define laserServer_hh

#include "miro/OdometryC.h"
#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

#include "SickLaserConnection.h"
#include "SickLaserStatistic.h"

namespace Laser
{
  class Parameters;
};

namespace Miro 
{
  class OdometryTracking;

  class LaserServer : public Server, 
		      public Log
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
};
#endif




