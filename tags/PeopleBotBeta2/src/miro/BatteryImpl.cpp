// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BatteryImpl.h"

namespace Miro
{
  // Implementation skeleton constructor
  BatteryImpl::BatteryImpl() :
    voltage_(-1.) // uninicialized value
  {
  }
  
  // Implementation skeleton destructor
  BatteryImpl::~BatteryImpl()
  {
  }
  
  void
  BatteryImpl::integrateData(double _voltage) ACE_THROW_SPEC(())
  {
    voltage_ = _voltage;
  }

  CORBA::Double 
  BatteryImpl::getVoltage() ACE_THROW_SPEC(())
  {
    return voltage_;
  }
};
