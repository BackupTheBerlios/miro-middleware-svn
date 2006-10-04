
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldTimer.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

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
