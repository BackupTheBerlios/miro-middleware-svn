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
#ifndef Video_AsynchBufferSet_h
#define Video_AsynchBufferSet_h

#include "VideoFilterLink.h"
#include "AsynchPreLink.h"

#include <miro/Synch.h>

// forward declaration
class ACE_Time_Value;

namespace Video
{
  // forward declaration
  class Device;
  class DeviceAsynchBufferSet;

  class AsynchBufferSet
  {
  public:
    // Initializing constructor.
    AsynchBufferSet(Miro::Mutex * _mutex,
		    Miro::Condition * _condition, Device const * _device);

    //! Accessor to the synchronous root node that is the id of the buffer set.
    Device const * device() const throw();
    //! Build up the buffer links.
    /** Returns entry id. */
    unsigned int addBufferLink(Filter * _filter, 
			       unsigned int _localIndex);
    //! Set synch mode.
    void allNew(bool _flag) throw();
    //! Set buffer.
    void buffer(unsigned int _id,
		ACE_Time_Value const& _imageTime,
		unsigned long _index, 
		unsigned char const * _buffer,
		FilterImageParameters const * _params);
    //! Test if there is a buffer set available.
    bool buffersComplete() const throw();
    //! Set the asynchronous buffers in the filter prelink vector.
    void setBuffers(PreLinkVector& _filterVector);
    //! Release the image buffers of the whole set.
    void releaseBufferSet(unsigned int _id) throw();
    //! Add connection to asynch predecessors.
    void protectedConnect();
    //! Remove connection to asynch predecessors.
    void protectedDisconnect();
    //! Calculate connectivity.
    /** 
     * This is called by the asynchronously connected predecessor.
     * This has to be protected by the global static connection lock. 
     */
    bool protectedCalcConnectivity() throw();
    //! Synchronous notification of connection calculation.
    void synchronouslyConnected(bool _flag) throw();
    //! Report connection status of filter.
    /** 
     * This is called by the asynchronously connected predecessor.
     * This has to be protected by the global static connection lock. 
     */
    bool connected() const throw();
    //! The index of the current buffer set.
    unsigned int current() const throw();


    void setDevAsynchSet(DeviceAsynchBufferSet * _deviceSet) throw();

    //! Accessor to the highest buffer number.
    unsigned int lastBufferInSet() const throw();

  protected:
    typedef std::vector<AsynchPreLink> AsynchPreLinkVector;

    //! Synchronisation with AsynchBufferBroker.
    Miro::Mutex * mutex_;
    Miro::Condition * condition_;
    //! Synchronous root node.
    Device const * device_;
    //! The index of the buffer set for the upcomming image.
    unsigned int upcoming_;
    //! The index of the buffer set for the current image.
    unsigned int current_;
    AsynchPreLinkVector buffer_[3];
    ACE_Time_Value stamp_[3];
    bool completed_[3];

    //! Invalidate buffers after processing.
    bool allNew_;
    //! Connection management.
    unsigned int connections_;
    //! Flag indicating we are connected.
    bool connected_;
    //! Flag indicating the connection status of the synchronous part.
    bool synchConnected_;

    //! Pointer to the device asynch link manager.
    DeviceAsynchBufferSet * deviceSet_;

    unsigned int firstBufferInSet_;
    unsigned int lastBufferInSet_;
  };

  inline
  Device const *
  AsynchBufferSet::device() const throw() {
    return device_;
  }

  inline
  bool
  AsynchBufferSet::connected() const throw() {
    return connected_;
  }

  inline
  unsigned int
  AsynchBufferSet::current() const throw() {
    return current_;
  }

  inline
  void
  AsynchBufferSet::setDevAsynchSet(DeviceAsynchBufferSet * _deviceSet) throw() {
    deviceSet_ = _deviceSet;
  }

  inline
  unsigned int
  AsynchBufferSet::lastBufferInSet() const throw() {
    return lastBufferInSet_;
  }
}
#endif // Video_AsynchBufferSet_h
