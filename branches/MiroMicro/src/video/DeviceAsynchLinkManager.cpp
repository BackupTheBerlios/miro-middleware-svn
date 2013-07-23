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
// $Id$
//
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
