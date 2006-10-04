
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: Laser360Connection.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------


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
    /// The laserTask is a group of threads (or only one) that handles
    /// complete pakets received by the laser. It does not have to bother
    /// for protocol issues. It is seperate thread, which discouples the
    /// hardware and protocol handling from the actual work to do.
    Laser360Task * laser360Task_;

    //---------------------------------------------------------------------------
    /// Reference to the parameter set. Must not be constant because
    /// the SickLaserTask might need to update some fields in the LaserDescription
    ::Laser360::Parameters & parameters_;

  };

};

#endif
