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


#include "PioneerStallImpl.h"

namespace Pioneer
{
  short rStall, lStall, batteryVol;

  // Implementation skeleton constructor
  StallImpl::StallImpl()
  {
    rStall=0;
    lStall=0;
  }
  
  // Implementation skeleton destructor
  StallImpl::~StallImpl()
  {
  }
  
  void
  StallImpl::integrateData(short rStalled, short lStalled, short bat)
  {
    rStall = rStalled;
    lStall = lStalled;
    batteryVol = bat;
  }

  CORBA::Short 
  StallImpl::getStalledWheels() throw ()
  {
    short erg;
    erg = 0;
    if (rStall) erg= 1;
    if (lStall) erg= erg + 2;
    return erg;
  }
  
  CORBA::Boolean 
  StallImpl::isStalled() throw ()
  {
    //Add your implementation here
    if (rStall || lStall) return true;
    else 
      return false;
  }
  
  CORBA::Short 
  StallImpl::getBattery() throw ()
  {
    return batteryVol;
  }

  void 
  StallImpl::unstall() throw ()
  {
    //Add your implementation here
  }
};
