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

#include "AsynchBufferSet.h"
#include "VideoFilter.h"
#include "BufferManager.h"
#include "DeviceAsynchBufferSet.h"

#include "miro/Synch.h"
#include "miro/Log.h"
#include "miro/TimeHelper.h"

#include <ace/Time_Value.h>
#include <climits>

namespace Video
{

  AsynchBufferSet::AsynchBufferSet(Miro::Mutex * _mutex,
				   Miro::Condition * _condition,
				   Device const * _device) :
    mutex_(_mutex),
    condition_(_condition),
    device_(_device),
    upcoming_(4),
    current_(4),
    allNew_(false),
    connections_(0),
    connected_(false),
    firstBufferInSet_(UINT_MAX),
    lastBufferInSet_(0)
  {
    for (unsigned int i = 0; i < 3; ++i) {
      stamp_[i] = ACE_Time_Value::zero;
      completed_[i] = false;
    }
  }

  unsigned int 
  AsynchBufferSet::addBufferLink(Filter * _filter, unsigned int _localIndex)
  {
    buffer_[0].push_back(AsynchPreLink(_filter, _localIndex));
    buffer_[1].push_back(AsynchPreLink(_filter, _localIndex));
    buffer_[2].push_back(AsynchPreLink(_filter, _localIndex));

    // if the last buffer is completed, the buffer set
    // is completed
    if (_filter->number() > lastBufferInSet_)
      lastBufferInSet_ = _filter->number();
    if (_filter->number() < firstBufferInSet_)
      firstBufferInSet_ = _filter->number();

    return buffer_[0].size() - 1;
  }

  void
  AsynchBufferSet::allNew(bool _flag) throw() 
  {
    allNew_ = _flag;
  }
  
  bool
  AsynchBufferSet::buffersComplete() const throw() {
    return completed_[deviceSet_->protectedLockedSet()];
  }

  //! Set buffer.
  void 
  AsynchBufferSet::buffer(unsigned int _id,
			  ACE_Time_Value const& _imageTime,
			  unsigned long _index, 
			  unsigned char const * _buffer,
			  FilterImageParameters const * _params)
  {
    //    std::cout << "Filter - AsynchBufferSet::buffer set" << std::endl;

    // on first buffer: find a new upcoming buffer set, set the time
    if (buffer_[0][_id].filter()->number() == firstBufferInSet_) {
      Miro::Guard guard(*mutex_);
      upcoming_ = deviceSet_->setUpcomingTime(_imageTime);
      stamp_[upcoming_] = _imageTime;
      completed_[upcoming_] = false;
    }

     // set the image
    //    cout << "upcoming: " << upcoming_ << endl;
    buffer_[upcoming_][_id].buffer(_index, _buffer, _params);

    // on last buffer: set completed
    if (buffer_[0][_id].filter()->number() == lastBufferInSet_) {

      Miro::Guard guard(*mutex_);
      // std::cout << "Filter - AsynchBufferSet::buffer ready - id" << std::endl;

      // the last buffer set is now obsolete 
      unsigned int oldCurrent = current_;
      current_ = upcoming_;
      completed_[current_] = true;
      unsigned int lockedSet =
	deviceSet_->setCompleted(buffer_[upcoming_][_id].filter()->number());
      if (oldCurrent != 4 &&
	  oldCurrent != lockedSet)
	releaseBufferSet(oldCurrent);
      condition_->broadcast();
    }
  }
 
  void 
  AsynchBufferSet::setBuffers(PreLinkVector& _filterVector)
  {
    unsigned int locked = deviceSet_->protectedLockedSet();
    AsynchPreLinkVector::iterator first, last = buffer_[locked].end();
    for (first = buffer_[locked].begin(); first != last; ++first) {
      MIRO_ASSERT(stamp_[locked] == deviceSet_->lockedStamp());
      first->setBufferPreLink(_filterVector);
      first->acquireBuffer();
    }

    if (allNew_ || 
	(locked != current_ && locked != upcoming_))
      releaseBufferSet(locked);
  }

  void
  AsynchBufferSet::releaseBufferSet(unsigned int _id) throw()
  {
    AsynchPreLinkVector::iterator first, last = buffer_[_id].end();
    for (first = buffer_[_id].begin(); first != last; ++first) {
      first->releaseBuffer();
    }
    completed_[_id] = false;
  }

  void
  AsynchBufferSet::protectedConnect() 
  {
    ++connections_;
    deviceSet_->protectedConnect();
    AsynchPreLinkVector::iterator first, last = buffer_[0].end();
    for (first = buffer_[0].begin(); first != last; ++first) {
      first->filter()->protectedConnect();
    }
  }
  void
  AsynchBufferSet::protectedDisconnect() 
  {
    AsynchPreLinkVector::reverse_iterator first, last = buffer_[0].rend();
    for (first = buffer_[0].rbegin(); first != last; ++first) {
      first->filter()->protectedDisconnect();
    }
    deviceSet_->protectedDisconnect();
    --connections_;
  }

  bool
  AsynchBufferSet::protectedCalcConnectivity() throw() {
    bool previouslyConnected = connected_;
    connected_ = connections_ > 0;

    if (!previouslyConnected || 
	connected_)
      return connected_;

    if (!synchConnected_) {
      releaseBufferSet(0);
      releaseBufferSet(1);
      releaseBufferSet(2);
      
      upcoming_ = 4;
      current_ = 4;
      
      if (!deviceSet_->connected()) {
	deviceSet_->clear();
      }
    }
    return false;
  }

  void
  AsynchBufferSet::synchronouslyConnected(bool _flag) throw() {

    if (!connected_ && !_flag && synchConnected_) {
      releaseBufferSet(0);
      releaseBufferSet(1);
      releaseBufferSet(2);
      
      upcoming_ = 4;
      current_ = 4;
      
      if (!deviceSet_->connected()) {
	deviceSet_->clear();
      }
    }

    synchConnected_ = _flag;
  }
}
