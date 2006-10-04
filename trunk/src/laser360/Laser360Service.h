
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: Laser360Service.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LASER_360_SERVICE_H_
  #define _LASER_360_SERVICE_H_

  #include "idl/ScanDescriptionS.h"
  #include "miro/Server.h"
  #include "miro/ReactorTask.h"
  #include "miro/StructuredPushSupplier.h"

  #include "Laser360Impl.h"
  #include "Laser360Connection.h"

  #include "Parameters.h"
  #include "miro/Utils.h"
  #include "miro/Log.h"

namespace Laser360
{
  class Parameters;
};

namespace Miro
{


  class LaserServer : public Server
  {
    typedef LaserServer Self;

    //---------------------------------------------------------------------------
    // DESCRIPTION
    //
    // This class starts up the LaserImpl registers it at the
    // naming service and runs the orb

  public:

    // Initialization and Termination methods.
    LaserServer( int argc, char * argv[] );
    // Constructor.

    ~LaserServer();
    // Destructor.

  protected:

    //---------------------------------------------------------------------------
    /// Sceduling parameters for a realtime thread
    ACE_Sched_Params schedparams_;

    //---------------------------------------------------------------------------
    ///CORBA reference to odometry for position stamps
    CosNotifyChannelAdmin::EventChannel_var ec_;

    StructuredPushSupplier * structuredPushSupplier_;

    Laser360Connection connection_;

    Laser360Impl pLaser_;

    Miro::Laser_var LaserObject_;

  };
};

#endif
