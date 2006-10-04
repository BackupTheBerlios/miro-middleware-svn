
//----------------------------------------------------------------
// Copyright (C) 2005
// Technische Universitaet Graz
//
// This file is part of the LD OEM Sick Scanner library
//----------------------------------------------------------------

//----------------------------------------------------------------
// project ....: Robot Processing Library
// file .......: Laser360Impl.h
// authors ....: Thomas Sega
// organization: TU - Graz
// creation ...: 02.12.2005 v1.0.0
// revisions ..:
//----------------------------------------------------------------

#ifndef _LASER_360_IMPL_H_
  #define _LASER_360_IMPL_H_

  #include "idl/LaserS.h"
  #include "idl/ScanDescriptionS.h"
  #include "miro/RangeSensorImpl.h"

namespace Miro
{
  //---------------------------------------------------------------------------
  /// Laser360Impl
  class Laser360Impl : public virtual POA_Miro::Laser, public virtual Miro::RangeSensorImpl
  {
  public:

    //---------------------------------------------------------------------------
    /// Constructor
    Laser360Impl( const Miro::ScanDescriptionIDL & _description, Miro::StructuredPushSupplier * _supplier = NULL );

    //---------------------------------------------------------------------------
    /// Destructor
    virtual ~Laser360Impl();

    //---------------------------------------------------------------------------
    /// Returns the latest laser scan, does not block. Returns garbage if no laserscan arrived so far.
    virtual LaserScanIDL getScan() throw();

    //---------------------------------------------------------------------------
    /// Blocks until a new laser scan arrives and then returns the data may block infinitely if no scans arrive.
    virtual LaserScanIDL getWaitScan() throw( ETimeOut );

  protected:
    void protectedGetScan( Miro::LaserScanIDL & scan ) const;

  };
};

#endif
