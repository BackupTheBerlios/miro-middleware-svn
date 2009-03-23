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
#ifndef _LD_TIMER_H_
  #define _LD_TIMER_H_

  #include <time.h>
  #include <unistd.h>
  #include "ldUserTypes.h"

//---------------------------------------------------------------------------
/// Timer uses the system's real-time clock to perform
/// nanosecond-resolution timing. In practical use, the maximum
/// resolution is never achieved, but accurate timings are still
/// possible.
/// @attention for linking you need to add the "-lrt" option!
namespace ldoem_
{

  class Timer
  {
  public:

    //---------------------------------------------------------------------------
    /// Start the timer. If the timer is already started, the previous
    /// start time is lost.
    void start()
    {
      clock_gettime( CLOCK_REALTIME, & _tmeStart );
      delay_set_ = false;
    }

    //---------------------------------------------------------------------------
    /// Start the timer. If the timer is already started, the previous
    /// start time is lost.
    /// @param delay_ms Sets the delay time for checking with isFinished method
    void start( const int32 delay_ms )
    {
      clock_gettime( CLOCK_REALTIME, & _tmeStart );
      delay_ms_ = delay_ms;
      delay_set_ = true;
    }

    //---------------------------------------------------------------------------
    /// Get the time elapsed from the last start() call. Time is
    /// measured in seconds.
    double getTime();

    //---------------------------------------------------------------------------
    /// Get the time elapsed from the last start() call.
    ///
    /// @param seconds where to store the number of seconds elapsed
    /// @param nanoseconds where to store the number of nanoseconds
    /// elapsed
    void getTime( int & seconds, long & nanoseconds );

    //---------------------------------------------------------------------------
    /// Checks if the delay time ( set with start( delay_ms ) is finished.
    /// @return TRUE if delay time is finished or delay isn't set.
    bool isFinished();

    //---------------------------------------------------------------------------
    /// Checks if the delay time ( set with start( delay_ms ) is finished.
    /// @return TRUE if delay time is finished or delay isn't set.
    void wait( const int32 delay_ms );

  private:

    //---------------------------------------------------------------------------
    // private data members
    //---------------------------------------------------------------------------

    /// struct for timer
    struct timespec _tmeStart;

    /// delay time in ms
    int32 delay_ms_;

    /// bool to set time
    bool delay_set_;
  }; // class Timer

};
#endif
