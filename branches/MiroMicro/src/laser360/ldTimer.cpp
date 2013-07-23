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
