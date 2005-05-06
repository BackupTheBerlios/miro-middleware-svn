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

#include "DeviceAsynchLinkManager.h"
#include "VideoFilterLink.h"
#include "VideoDevice.h"
#include "DeviceAsynchBufferSet.h"

namespace Video
{
  DeviceAsynchLinkManager::DeviceAsynchLinkManager() :
    mutex_(),
    condition_(mutex_)
  {}

  DeviceAsynchLinkManager::~DeviceAsynchLinkManager() 
  {
  }
  
  DeviceAsynchBufferSet *
  DeviceAsynchLinkManager::addDeviceLink(Device * _device) throw()
  {
    DeviceAsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      if (_device == first->device()) {
	return &(*first);
      }
    }
    bufferSets_.push_back(DeviceAsynchBufferSet(_device, &mutex_, &condition_));
    return &bufferSets_.back();
  }

  bool
  DeviceAsynchLinkManager::asynchBuffersConnected(ACE_Time_Value const& _stamp)
    throw() 
  {
    DeviceAsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      if (first->needsConnectivityUpdate()) {
	if (first->device()->connectivityTimeStamp() <= _stamp) {
	  return false;
	}
	else {
	  first->needsConnectivityUpdate(false);
	}
      }
    }
    return true;
  }

  void
  DeviceAsynchLinkManager::synchMode(bool _synchWithCurrent,
				     bool _synchAllNew) throw() {
    DeviceAsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      first->synchMode(_synchWithCurrent, _synchAllNew);
    }
  }
}
