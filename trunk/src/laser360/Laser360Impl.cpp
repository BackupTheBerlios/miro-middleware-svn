
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: Laser360Impl.cpp
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

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
