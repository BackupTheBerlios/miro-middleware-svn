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
#ifndef TimerEventHandler_hh
#define TimerEventHandler_hh

#include <ace/Event_Handler.h>

namespace FaulMotor
{
  // forward declarations
  class Connection;

  //! Timed Event for communication.
  /** 
   * We have to poll the odometry values. But we can't poll while we
   * set a motor command. So in order to gain equidistant odometry
   * probes, we write a sequence of an acceleration, a velocity and a
   * odomertry polling command every time.
   */
  class TimerEventHandler : public ACE_Event_Handler
  {
    //! Super class accessor.
    typedef ACE_Event_Handler Super;

  public:
    //! Initializing consructor.
    /** 
     * We need a reference to the connection, in order
     * to send the commands.
     */
    TimerEventHandler(Connection& _connection);
    //! Virtual constructor for cleanup.
    virtual ~TimerEventHandler();

    //! Inherited callback method.
    virtual int handle_timeout (const ACE_Time_Value &current_time,
				const void *act = 0);

  protected:
    //! Reference to the connection to send the commands.
    Connection& connection_;
    //! Counter to track failures of odometry responses.
    /** 
     * Those can happen due to latencies, but ongoing failure
     * indicates a crash of the lowlevel controllers.
     */
    unsigned int stallCounter_;
  };
}

#endif
