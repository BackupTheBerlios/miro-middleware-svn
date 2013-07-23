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
#ifndef Video_DeviceAsynchLinkManager_h
#define Video_DeviceAsynchLinkManager_h

#include "VideoFilterLink.h"
#include "DeviceAsynchBufferSet.h"
#include "AsynchSuccLink.h"

#include "miro/Log.h"

#include <list>

namespace Video
{
  // forward declarations
  class Filter;

  //! Manager for links to asynchronous successors.
  /*
   * The links are grouped by device (root node). As every device
   * is one synchronous thread.
   */
  class DeviceAsynchLinkManager
  {
  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    //! Initializing constructor.
    DeviceAsynchLinkManager();
    //! Free remaining asynch buffers.
    ~DeviceAsynchLinkManager();

    //! Deferred initialization.
    void synchMode(bool _synchWithCurrent, bool _synchAllNew) throw();

    //! Clear connection status for calculate connectivity call backs.
    void clearConnectivityStatus() throw();
    //! Set a device to be connected.
    void connect(Device const * _device,
		 unsigned int _maxBufferId) throw();
    //! Set flag to wait for connectivity update for the indicated device.
    void needsConnectivityUpdate(Device const * _device) throw();

    //! Test if all asynch devices are updated accordingly.
    /** Has to be called with Video::Filter::connectionMutex_ held! */
    bool asynchBuffersConnected(ACE_Time_Value const& _stamp) throw();

    //! Returns true if all buffer sets are available under the current synch mode.
    bool tryAcquireBufferSets(ACE_Time_Value const& _maxJitter) throw();
    void releaseBufferSets() throw();

    //! Build up the buffer links.
    DeviceAsynchBufferSet * addDeviceLink(Device * _device) throw();
    //! Test if buffers are available.
    bool tryGetAsynchBuffers(PreLinkVector& _filterVector);

  protected:

    //--------------------------------------------------------------------------
    // protected types
    //--------------------------------------------------------------------------

    typedef std::list<DeviceAsynchBufferSet> DeviceAsynchBufferSetVector;

    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------

    //! Synchronize with all buffers.
    Miro::Mutex mutex_;
    //! Wait for completion of buffers.
    Miro::Condition condition_;
    //! One buffer set per device.
    DeviceAsynchBufferSetVector bufferSets_;
  };

  inline
  void
  DeviceAsynchLinkManager::clearConnectivityStatus() throw() {
    DeviceAsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      first->clearMaxFilterId();
    }
  }

  inline
  void
  DeviceAsynchLinkManager::connect(Device const * _device,
				   unsigned int _maxBufferId) throw() {
    DeviceAsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      if (first->device() == _device) {
	first->setMaxFilterId(_maxBufferId);
	return;
      }
    }
    MIRO_ASSERT(false);
  }

  inline
  void
  DeviceAsynchLinkManager::needsConnectivityUpdate(Device const * _device) throw() {
    DeviceAsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      if (first->device() == _device) {
	first->needsConnectivityUpdate(true);
	return;
      }
    }
    MIRO_ASSERT(false);
  }

  inline
  bool
  DeviceAsynchLinkManager::tryAcquireBufferSets(ACE_Time_Value const& _maxJitter) throw() {
    if (bufferSets_.size() == 0)
      return true;

    Miro::Guard guard(mutex_);
    DeviceAsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {

      //TODO: is it valid to use connected here?
      if (first->connected() &&
	  !first->tryAcquireBufferSet(_maxJitter)) {

	// release already acquired buffers
	while (first != bufferSets_.begin()) {
	  --first;
	  first->releaseBufferSet(true);
	}

	return false;
      }
    }
    return true;
  }

  inline
  void
  DeviceAsynchLinkManager::releaseBufferSets() throw() {
    Miro::Guard guard(mutex_);
    DeviceAsynchBufferSetVector::reverse_iterator first, last = bufferSets_.rend();
    for (first = bufferSets_.rbegin(); first != last; ++first) {
      first->releaseBufferSet();
    }
  }
}
#endif // Video_DeviceAsynchLinkManager_h
