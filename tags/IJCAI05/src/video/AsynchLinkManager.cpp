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

#include "config.h"

#include "AsynchLinkManager.h"
#include "VideoFilterLink.h"
#include "VideoDevice.h"
#include "AsynchBufferSet.h"
#include "VideoDevice.h"
#include "DeviceAsynchLinkManager.h"

namespace Video
{
  AsynchLinkManager::AsynchLinkManager() :
    mutex_(),
    condition_(mutex_),
    ownDeviceAsynchLinkManager_(NULL)
  {}
  
  AsynchLinkManager::~AsynchLinkManager()
  {
  }
  
  AsynchSuccLink
  AsynchLinkManager::addBufferLink(Filter * _filter, unsigned int _index) {
    AsynchBufferSet * s = NULL;
    unsigned int id = 255;
    
    AsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      if (_filter->rootDevice() == first->device()) {
#if GCC_MAJOR_VERSION >= 3
	s = first.base();
#else
	s = first;
#endif
	id = first->addBufferLink(_filter, _index);
	break;
      }
    }

    if (first == last) {
      bufferSets_.
	push_back(AsynchBufferSet(&mutex_, &condition_,
				  dynamic_cast<Device const *>(_filter->rootNode())));
      s = &bufferSets_.back();
      id = bufferSets_.back().addBufferLink(_filter, _index);
    }
    
    // add to device link manager
    DeviceAsynchBufferSet * deviceSet = 
      ownDeviceAsynchLinkManager_->addDeviceLink(_filter->rootDevice());
    s->setDevAsynchSet(deviceSet);
    return AsynchSuccLink(s, id);
  }
  
  void
  AsynchLinkManager::deviceAsynchLinkManager(DeviceAsynchLinkManager * _manager)
    throw() 
  {
    ownDeviceAsynchLinkManager_ = _manager;
  }

  void
  AsynchLinkManager::setSynchMode(bool _synchWithCurrent, bool _synchAllNew) throw() {
    AsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      first->allNew(_synchAllNew);
    }

    synchWithCurrent_ = _synchWithCurrent;
  }

  void 
  AsynchLinkManager::getAsynchBuffers(PreLinkVector& _filterVector)
  {
    if (bufferSets_.size() == 0)
      return;

    AsynchBufferSetVector::iterator first, last = bufferSets_.end();

    if (!synchWithCurrent_) {
      Miro::Guard guard(mutex_);
      for (first = bufferSets_.begin(); first != last; ++first) {
	ACE_Time_Value maxWait = ACE_OS::gettimeofday() + ACE_Time_Value(0, 500000);
	while (!first->buffersComplete()) {
	  if (condition_.wait(&maxWait) == -1) {
	    MIRO_LOG_OSTR(LL_WARNING, "AsynchLinkManager: Timeout on wait asynch buffer completion.");
	    maxWait = ACE_OS::gettimeofday() + ACE_Time_Value(0, 500000);
	  }
	}
      }
    }

    for (first = bufferSets_.begin(); first != last; ++first) {
      first->setBuffers(_filterVector);
    }
  }

  void
  AsynchLinkManager::setDeviceAsynchLinksConnected() {
    AsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      ownDeviceAsynchLinkManager_->connect(first->device(), 
					   first->lastBufferInSet());
    }
  }

  void
  AsynchLinkManager::protectedConnect() {
    AsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      first->protectedConnect();
      ownDeviceAsynchLinkManager_->needsConnectivityUpdate(first->device());
    }
  }
}
