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
#ifndef laserImpl_hh
#define laserImpl_hh

#include "miro/LaserS.h"
#include "miro/RangeSensorImpl.h"

namespace Miro 
{
  class OdometryTracking;

  /**
   * LaserImpl
   */
  class LaserImpl :  public virtual POA_Miro::Laser, 
		     public virtual Miro::RangeSensorImpl
  {
  public:
    /**
     * Constructor
     */
    LaserImpl(OdometryTracking * _odoTracking,
		       const Miro::ScanDescriptionIDL& _description,
		       Miro::StructuredPushSupplier * _supplier = NULL);

    /**
     * Destructor
     */
    virtual ~LaserImpl();

    /**
     * Returns the latest laser scan, does not block.
     * Returns garbage if no laserscan arrived so far.
     */
    virtual LaserScanIDL getScan() throw();

    /**
     * Blocks until a new laser scan arrives and then returns the data
     * may block infinitely if no scans arrive.
     */
    virtual LaserScanIDL getWaitScan() throw(ETimeOut);

  protected:
    void protectedGetScan(Miro::LaserScanIDL& scan) const;

    OdometryTracking * odoTracking_;
  };
};
#endif




