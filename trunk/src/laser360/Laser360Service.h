// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
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

    Laser360Impl pLaser_;

    Miro::Laser_var LaserObject_;

    Laser360Connection connection_;

  };
};

#endif
