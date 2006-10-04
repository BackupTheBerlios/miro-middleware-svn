
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: Laser360Task.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LASER_360_TASK_H_
  #define _LASER_360_TASK_H_

  #include "ldScannerControl.h"
  #include "miro/Thread.h"
  #include "miro/RangeSensorImpl.h"

namespace Laser360
{
  class Parameters;
}


namespace Miro
{

  // forward declaration
  class RangeSensorImpl;


  class Laser360Task : public Thread
  {

  public:

    //---------------------------------------------------------------------------
    /// Constructor
    Laser360Task( RangeSensorImpl & _rangeSenorI );

    //---------------------------------------------------------------------------
    /// Destructor
    virtual ~Laser360Task();

    //---------------------------------------------------------------------------
    /// this is the task main loop, which defines the work to be done
    /// this is worked upon on the number of threads that are created inside this task
    virtual int svc();

  protected :

    //---------------------------------------------------------------------------
    /// pointer to laser control
    ldoem_::ScannerControl * control_;

  };
};

#endif
