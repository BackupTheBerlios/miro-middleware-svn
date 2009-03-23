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
