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
#ifndef Video_AsynchLinkManager_h
#define Video_AsynchLinkManager_h

#include "VideoFilterLink.h"
#include "AsynchBufferSet.h"
#include "AsynchSuccLink.h"

namespace Video
{
  // forward declarations
  class Filter;
  class DeviceAsynchLinkManager;

  //! Manager for links to asynchronous successors.
  /*
   * The links are grouped by device (root node). As every device
   * is one synchronous thread.
   */
  class AsynchLinkManager
  {
  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    //! Initializing constructor.
    AsynchLinkManager();
    //! Free remaining asynch buffers.
    ~AsynchLinkManager();

    //! Test if there are asynchronous buffer links.
    bool empty() const throw();

    //! Deferred initialization: Set synch mode.
    /** Default is false. */
    void setSynchMode(bool _synchWithCurrent, bool _synchAllNew) throw();
    //! Deferred initialization.
    /** Set device asynch link manager. */
    void deviceAsynchLinkManager(DeviceAsynchLinkManager * _manager) throw();
    //! Build up the buffer links.
    AsynchSuccLink addBufferLink(Filter * _filter, unsigned int _index);
    //! Propagate a connection to all asynchronous predecessors.
    /** This has to be protected by the global static connection lock. */
    void protectedConnect();
    //! Propagate a disconnection to all asynchronous predecessors.
    /**
     * This is locked under the global static connection lock.
     * Free buffers on inactivation. 
     */
    void protectedDisconnect();
    //! Synchronous notification of connection calculation.
    void synchronouslyConnected(bool _flag) throw();

    //! Get asynch buffers.
    /** If these ain't available yet, we do wait for them. */
    void getAsynchBuffers(PreLinkVector& _filterVector);

    //! Set the devices to be connected in the DeviceAsynchLinkManager.
    void setDeviceAsynchLinksConnected();
    
  protected:

    //--------------------------------------------------------------------------
    // protected types
    //--------------------------------------------------------------------------

    typedef std::vector<AsynchBufferSet> AsynchBufferSetVector;

    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------

    //! Synchronize with all buffers.
    Miro::Mutex mutex_;
    //! Condition to broadcast newly finished buffers.
    Miro::Condition condition_;
    //! One buffer set per device.
    AsynchBufferSetVector bufferSets_;
    //! Back link to the root nodes global asynch buffer manager.
    DeviceAsynchLinkManager * ownDeviceAsynchLinkManager_;

    //! Synch mode.
    bool synchWithCurrent_;
  };

  inline
  bool 
  AsynchLinkManager::empty() const throw() {
    return 0 == bufferSets_.size();
  }

  inline
  void
  AsynchLinkManager::protectedDisconnect() {
    AsynchBufferSetVector::reverse_iterator first, last = bufferSets_.rend();
    for (first = bufferSets_.rbegin(); first != last; ++first) {
      first->protectedDisconnect();
    }
  }
  inline
  void
  AsynchLinkManager::synchronouslyConnected(bool _flag) throw() {
    AsynchBufferSetVector::iterator first, last = bufferSets_.end();
    for (first = bufferSets_.begin(); first != last; ++first) {
      first->synchronouslyConnected(_flag);
    }
  }
}
#endif // Video_AsynchLinkManager_h
