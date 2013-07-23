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
#ifndef _LASER_360_CONNECTION_H_
  #define _LASER_360_CONNECTION_H_

  #include "miro/RangeSensorImpl.h"
  #include "idl/LaserC.h"

  #include "Laser360Task.h"
  #include "ldScannerControl.h"

  #include <ace/Event_Handler.h>
  #include <ace/Synch.h>


namespace Laser360
{
  class Parameters;
}

namespace Miro
{

  // forward declaration
  class RangeSensorImpl;
  class Laser360Task;


  //---------------------------------------------------------------------------
  /// LaserConnection
  class Laser360Connection
  {

  public:

    //---------------------------------------------------------------------------
    /// Constructor
    Laser360Connection( RangeSensorImpl & _rangeSenorI );

    //---------------------------------------------------------------------------
    /// Destructor
    virtual ~Laser360Connection();

    //---------------------------------------------------------------------------
    /// Stop all action of tasks and deregister event handlers.
    void stopTasks();

  protected:


    //---------------------------------------------------------------------------
    /// Reference to the parameter set. Must not be constant because
    /// the SickLaserTask might need to update some fields in the LaserDescription
    ::Laser360::Parameters & parameters_;

    //---------------------------------------------------------------------------
    /// The laserTask is a group of threads (or only one) that handles
    /// complete pakets received by the laser. It does not have to bother
    /// for protocol issues. It is seperate thread, which discouples the
    /// hardware and protocol handling from the actual work to do.
    Laser360Task * laser360Task_;


  };

};

#endif
