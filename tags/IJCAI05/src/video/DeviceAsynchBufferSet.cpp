// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DeviceAsynchBufferSet.h"

namespace Video
{
  DeviceAsynchBufferSet::DeviceAsynchBufferSet(Device const * _device,
					       Miro::Mutex * _mutex,
					       Miro::Condition * _condition) :
    device_(_device),
    connected_(false),
    connections_(0),
    needsConnectivityUpdate_(false),
    mutex_(_mutex),
    condition_(_condition),
    upcomingSet_(4),
    currentSet_(4),
    lockedSet_(4)
  {
    for (unsigned int i = 0; i < 3; ++i) {
      stamp_[i] = ACE_Time_Value::zero;
    }
  }

  void
  DeviceAsynchBufferSet::synchMode(bool _synchWithCurrent,
				   bool _synchAllNew) throw() 
  {
    synchWithCurrent_ = _synchWithCurrent;
    synchAllNew_ = _synchAllNew;
  }
}
