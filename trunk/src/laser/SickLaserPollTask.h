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
#ifndef laserPollTask_hh
#define laserPollTask_hh

#include "miro/Thread.h"

namespace Laser
{
  class Parameters;
};

namespace Miro 
{
  // forward declaration
  class LaserConnection;
  class LaserEvent;
  class LaserStatistic;

  class LaserPollTask : public Thread
  {
  public:
    //! Constructor
    LaserPollTask(LaserConnection& _laser,
		  LaserEvent& _event,
		  LaserStatistic * _laserStatistic);

    //! Destructor
    virtual ~LaserPollTask();

    /**
     * this is the task main loop, which defines the work to be done
     * this is worked upon on the number of threads that are created
     * inside this task
     */
    virtual int svc ();

  protected:
    /**
     * reference to the LaserImpl class, for signaling data
     * via condition variables
     */
    LaserConnection& laser;
    LaserEvent&      event;
    const ::Laser::Parameters& description;
    LaserStatistic * laserStatistic;
  };

};


#endif




