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
// $Id: SickLaserImpl.cpp,v 1.3 2007-02-25 07:09:42 hutz Exp $
//
#include "SickLaserImpl.h"

#include "miro/OdometryTracking.h"
#include "miro/TimeHelper.h"

#undef DEBUG

namespace Miro
{
  using std::string;

  /***************************************************************************
   *
   *  Laser Implementation 
   *
   ***************************************************************************/

  LaserImpl::LaserImpl(OdometryTracking * _odoTracking,
		       const Miro::ScanDescriptionIDL& _description,
		       Miro::StructuredPushSupplier * _supplier) :
    RangeSensorImpl(_description, _supplier),
    odoTracking_(_odoTracking)
  {
    maxWait_ = ACE_Time_Value(5); // five seconds
  }



  LaserImpl::~LaserImpl()
  {
  }

  Miro::LaserScanIDL 
  LaserImpl::getScan() throw()
  {
    Guard guard(mutex_);
    Miro::LaserScanIDL scan;

    protectedGetScan(scan);
    return scan;
  }

  Miro::LaserScanIDL 
  LaserImpl::getWaitScan() throw(Miro::ETimeOut)
  {
    Guard guard(mutex_);
    Miro::LaserScanIDL scan;

    ACE_Time_Value timeout = ACE_OS::gettimeofday();
    timeout += maxWait_;

    if (condition_.wait(&timeout) < 0) 
      throw Miro::ETimeOut("LaserImpl::getWaitScan");

    protectedGetScan(scan);
    return scan;
  }

  void 
  LaserImpl::protectedGetScan(Miro::LaserScanIDL& scan) const
  {
    scan.time = scan_.time;
    scan.positionValid = false;

    if (odoTracking_) {
      ACE_Time_Value timeStamp;
      timeC2A(scan_.time, timeStamp);

      scan.positionValid = true;    
      scan.position = odoTracking_->getPosition(timeStamp).position;
    }

    // copy the scan data
    for (long i = std::max(360, (int)scan_.range[0].length() - 1); i >= 0; --i) {
      scan.value[i] = scan_.range[0][i];
    }
  }
};
