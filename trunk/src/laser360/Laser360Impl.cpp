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
#include "Laser360Impl.h"


namespace Miro
{

  //---------------------------------------------------------------------------
  Laser360Impl::Laser360Impl( const Miro::ScanDescriptionIDL & _description, Miro::StructuredPushSupplier * _supplier ) :
     RangeSensorImpl( _description, _supplier )
     {
       maxWait_ = ACE_Time_Value( 5 ); // five seconds
  }

  //---------------------------------------------------------------------------
  Laser360Impl::~Laser360Impl()
  {


  }

  Miro::LaserScanIDL  Laser360Impl::getScan() throw()
  {
    Guard guard( mutex_ );
    Miro::LaserScanIDL scan;

    protectedGetScan( scan );
    return scan;
  }

  Miro::LaserScanIDL  Laser360Impl::getWaitScan() throw( Miro::ETimeOut )
  {
    Guard guard( mutex_ );
    Miro::LaserScanIDL scan;

    ACE_Time_Value timeout = ACE_OS::gettimeofday();
    timeout += maxWait_;

    if ( condition_.wait( & timeout ) < 0 )
      throw Miro::ETimeOut( "LaserImpl::getWaitScan" );

    protectedGetScan( scan );
    return scan;
  }

  void  Laser360Impl::protectedGetScan( Miro::LaserScanIDL & scan ) const
  {
    ACE_Time_Value timeStamp;

    scan.time = scan_.time;
    scan.positionValid = false;

    // copy the scan data
    for ( long i = std::max( 360, ( int )scan_.range[0].length() - 1 ); i >= 0; --i )
    {
      scan.value[i] = scan_.range[0] [i];
    }

  }


};
