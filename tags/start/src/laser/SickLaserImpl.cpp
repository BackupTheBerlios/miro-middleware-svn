// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


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
    maxWait = ACE_Time_Value(5); // five seconds
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
    timeout += maxWait;

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
