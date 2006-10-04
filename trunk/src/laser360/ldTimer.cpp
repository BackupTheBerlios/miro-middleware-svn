
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: ldTimer.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#include <iostream>
#include "ldTimer.h"

namespace ldoem_
{

  //---------------------------------------------------------------------------
  double Timer::getTime()
  {
    struct timespec t;
    clock_gettime( CLOCK_REALTIME, & t );
    return ( t.tv_sec - _tmeStart.tv_sec + double( t.tv_nsec ) / 1000000000.0
       - double( _tmeStart.tv_nsec ) / 1000000000.0 );
  }

  //---------------------------------------------------------------------------
  void Timer::getTime( int & secs, long & nanosecs )
  {
    struct timespec t;
    clock_gettime( CLOCK_REALTIME, & t );

    if ( t.tv_nsec >= _tmeStart.tv_nsec )
    {
      secs = t.tv_sec - _tmeStart.tv_sec;
      nanosecs = t.tv_nsec - _tmeStart.tv_nsec;
    }
    else
    {
      secs = t.tv_sec - _tmeStart.tv_sec - 1;
      nanosecs = 1000000000L + t.tv_nsec - _tmeStart.tv_nsec;
    }
  }

  //---------------------------------------------------------------------------
  bool Timer::isFinished()
  {
    struct timespec t;
    int64 diff;

    if ( !delay_set_ )
      return true;

    clock_gettime( CLOCK_REALTIME, & t );

    diff = ( t.tv_sec - _tmeStart.tv_sec ) * 1000000 + ( t.tv_nsec - _tmeStart.tv_nsec ) / 1000;
    if ( diff >= delay_ms_ * 1000 )
      return true;
    return false;
  }

  //---------------------------------------------------------------------------
  void Timer::wait( const int32 delay_ms )
  {
    usleep( delay_ms * 1000 );
  }

};
