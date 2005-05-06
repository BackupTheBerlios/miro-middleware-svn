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
#ifndef Video_DeviceAsynchBufferSet_h
#define Video_DeviceAsynchBufferSet_h

#include "VideoFilterLink.h"

#include "miro/Synch.h"
#include "miro/Log.h"
#include "miro/TimeHelper.h"

#include <ace/Time_Value.h>

// forward declaration
class ACE_Time_Value;

namespace Video
{
  // forward declaration
  class Device;

  class DeviceAsynchBufferSet
  {
  public:
    //! Initializing constructor.
    DeviceAsynchBufferSet(Device const * _device,
			  Miro::Mutex * _mutex, Miro::Condition * _condition);

    //! Deferred initialization.
    void synchMode(bool _synchWithCurrent,
		   bool _synchAllNew) throw();
    //! Clean up on disconnection.
    void clear() throw();

    //! Accessor to the synchronous root node that is the id of the buffer set.
    Device const * device() const throw();
    //! Add connection to asynch predecessors.
    void protectedConnect() throw();
    //! Remove connection to asynch predecessors.
    void protectedDisconnect() throw();
    //! Report connection status of filter.
    /** 
     * This is called by the device asynch link manager.
     * This has to be protected by the global static connection lock. 
     */
    bool connected() const throw();
    //! Test if we need to wait for a connecitvity update from the asynchronous device.
    bool needsConnectivityUpdate() const throw();

    //! Indicate, that we need to wait for a connecitvity update from the asynchronous device.
    void needsConnectivityUpdate(bool _flag) throw();

    unsigned int setUpcomingTime(ACE_Time_Value const& _stamp) throw();
    unsigned int setCompleted(unsigned int _id) throw();
    ACE_Time_Value currentSet() throw();
    //! Try to acquire the buffer set.
    bool tryAcquireBufferSet() throw();
    void releaseBufferSet(bool _keepAllways = false) throw();

    //! Index of the locked set.
    /** This is not protected, as it is called from synchronous buffers. */
    unsigned int protectedLockedSet() const throw();

    //! Clear the maximum filter id for recalculation.
    void clearMaxFilterId() throw();
    /** 
     * This is called by the asynch buffer sets on connectivity calibration.
     * This has to be protected by the global static connection lock. 
     */
    void setMaxFilterId(unsigned int _id) throw();

    //! Time stamp of the locked set.
    /** This is not protected, as it is called from synchronous buffers. */
    ACE_Time_Value const& lockedStamp() const throw();

  protected:
    //! Synchronous root node.
    Device const * device_;
    //! Flag indicating we are connected.
    bool connected_;
    //! Number of active connections.
    unsigned int connections_;
    //! The maximum filter id, that is connected.
    unsigned int maxFilterId_;
    //! Flag inidicating that we need to wait for a connectivity update of this device.
    bool needsConnectivityUpdate_;

    //! Mutex for time stamp synching.
    Miro::Mutex * mutex_;
    //! Condition variable for buffer synching.
    Miro::Condition * condition_;

    //! Index of the upcoming buffer set.
    unsigned int upcomingSet_;
    //! Index of the current buffer set.
    unsigned int currentSet_;
    //! Index of the locked buffer set.
    unsigned int lockedSet_;
    //! The time stamp of the image sets.
    ACE_Time_Value stamp_[3];

    //! Flag indication synchronization mode.
    /**
     * Take the currently completed buffer set for processing. Otherwise 
     * also uncompleted buffer sets can be used for synchronization.
     * - synch with current in combination with a low priority will
     *   ensure undisturbed processing of the asynch predecessor
     *   devices.
     * - synch with upcoming in combination with a high priority will
     *   minimize the diversion of time stamps of multiple image sources
     *   for stereo image processing.
     */
    bool synchWithCurrent_;
    //! Flag indication synchronization mode.
    /**
     * The asynch buffers are freed after processing. This allows
     * faster reclaiming of buffers by the other devices. But it also
     * synchronizes the device with the slowest buffer it is connected
     * to.
     */
    bool synchAllNew_;
  };

  inline
  Device const *
  DeviceAsynchBufferSet::device() const throw() {
    return device_;
  }

  inline
  bool
  DeviceAsynchBufferSet::connected() const throw() {
    return connected_;
  }

  inline
  void
  DeviceAsynchBufferSet::needsConnectivityUpdate(bool _flag) throw() {
    needsConnectivityUpdate_ = _flag;
  }

  inline
  bool
  DeviceAsynchBufferSet::needsConnectivityUpdate() const throw() {
    return needsConnectivityUpdate_;
  }
  
  inline
  unsigned int 
  DeviceAsynchBufferSet::setUpcomingTime(ACE_Time_Value const& _stamp) throw() {
    if (upcomingSet_ == 4 ||
	stamp_[upcomingSet_] != _stamp) {
      Miro::Guard guard(*mutex_);
      // find new upcoming set index:

      // witch slot can we take as new upcoming buffer set?
      if (upcomingSet_ != 0 && lockedSet_ != 0) {
	// take slot 0
	upcomingSet_ = 0;
      }
      else if (upcomingSet_ != 1 && lockedSet_ != 1) {
	// take slot 1
	upcomingSet_ = 1;
      }
      else {
	// take slot 2
	MIRO_ASSERT(upcomingSet_ != 2 && lockedSet_ != 2);
	upcomingSet_ = 2;
      }

      stamp_[upcomingSet_] = _stamp;
    }

    return upcomingSet_;
  }

  /** Called by asynch buffer. */
  inline
  unsigned int 
  DeviceAsynchBufferSet::setCompleted(unsigned int _id) throw() {

    //    cout << "trying to set upcoming complete for:" << _id  << endl;

    // if we have a race on this class variable we are in the middle of a
    // connectivity update.
    // but then no synchronous filter will wait on condition_ or hold mutex_
    Miro::Guard guard(*mutex_);
    if (_id == maxFilterId_) {
      
      //cout << "setting upcoming complete: " << upcomingSet_ << endl;

      currentSet_ = upcomingSet_;
      if (synchWithCurrent_)
	condition_->broadcast();
    }
    return lockedSet_;
  }

  inline
  ACE_Time_Value
  DeviceAsynchBufferSet::currentSet() throw() {
    Miro::Guard guard(*mutex_);
    return stamp_[currentSet_];
  }

  /** This is called by DeviceAsynchLinkManager which already holds the lock. */
  inline
  bool
  DeviceAsynchBufferSet::tryAcquireBufferSet() throw() {
    if (synchWithCurrent_) {
      if (currentSet_ == 4 ||
	  stamp_[currentSet_] == ACE_Time_Value::zero) {
	return false;
      }
      lockedSet_ = currentSet_;
    }
    else {
      if (upcomingSet_ == 4 ||
	  stamp_[upcomingSet_] == ACE_Time_Value::zero) {
	return false;
      }
      lockedSet_ = upcomingSet_;
    }
    //    cout << "acquired locked set: " << lockedSet_ << endl;
    return true;
  }

  /** This is called by DeviceAsynchLinkManager which already holds the lock. */
  inline
  void 
  DeviceAsynchBufferSet::releaseBufferSet(bool _keepAllways) throw() {
    //    cout << "release set: " << lockedSet_ << endl;

    // if we process each image only once, we clear the set
    if (!_keepAllways && synchAllNew_ && lockedSet_ != 4) {
      stamp_[lockedSet_] = ACE_Time_Value::zero;
    }
    lockedSet_ = 4;
  }

  inline
  void 
  DeviceAsynchBufferSet::clearMaxFilterId() throw() {
    maxFilterId_ = 0;
    connected_ = connections_ > 0;
  }

  inline
  void
  DeviceAsynchBufferSet::setMaxFilterId(unsigned int _id) throw() {
    if (_id > maxFilterId_)
      maxFilterId_ = _id;
  }

  inline
  unsigned int
  DeviceAsynchBufferSet::protectedLockedSet() const throw() {
    return lockedSet_;
  }

  inline
  ACE_Time_Value const&
  DeviceAsynchBufferSet::lockedStamp() const throw() {
    MIRO_ASSERT(lockedSet_ != 4);
    return stamp_[lockedSet_];
  }
  inline
  void
  DeviceAsynchBufferSet::protectedConnect() throw() {
    if (connections_ == 0) {
      MIRO_ASSERT(upcomingSet_ == 4);
      MIRO_ASSERT(currentSet_ == 4);
      MIRO_ASSERT(lockedSet_ == 4);
    }

    ++connections_;
  }
  inline
  void
  DeviceAsynchBufferSet::protectedDisconnect() throw() {
    --connections_;
  }

  inline
  void
  DeviceAsynchBufferSet::clear() throw() {
    // reset buffer set on disconnection
    //    cout << "\areset buffer set on disconnection\n";
    upcomingSet_ = 4;
    currentSet_ = 4;
    MIRO_ASSERT(lockedSet_ == 4);
    
    stamp_[0] = ACE_Time_Value::zero;
    stamp_[1] = ACE_Time_Value::zero;
    stamp_[2] = ACE_Time_Value::zero;
  }

}
#endif // Video_DeviceAsynchBufferSet_h
