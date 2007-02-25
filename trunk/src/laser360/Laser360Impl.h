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
