// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "BufferManager.h"

namespace Video
{
  /**
   * @param buffers Number of buffers.
   * @param bufferSize The size of each buffer.
   * @param memory Memory area, holding space for all buffers.  If
   * NULL, the BufferManager will allocate and manage the memory by
   * himself.
   */
  BufferManager::BufferManager(unsigned int _buffers, 
			       unsigned int _bufferSize,
			       unsigned char * _memory)  throw (std::bad_alloc) :
    buffers_(_buffers),
    bufferSize_(_bufferSize),
    memory_(_memory),
    owner_(false),
    cond_(mutex_),
    currentBuffer_(0),
    bufferStatus_(buffers_)
  {
    if (memory_ == NULL) {
      memory_ = new unsigned char[bufferSize_ * buffers_];
      owner_ = true;
    }

    unsigned int offset = 0;
    BufferVector::iterator first, last = bufferStatus_.end();
    for (first = bufferStatus_.begin(); first != last; ++first) {
      first->buffer = memory_ + offset;
      offset += bufferSize_;
    }
  }

  /**
   * @param buffers Number of buffers.
   * @param bufferAddr An array of size buffers, holding the
   *  address for each of the buffers.
   * @param memory Base address of the memory area, holding space for all buffers.
   */
  BufferManager::BufferManager(unsigned int _buffers, 
			       unsigned char * _bufferAddr[]) :
    buffers_(_buffers),
    bufferSize_(0),
    memory_(NULL),
    owner_(false),
    cond_(mutex_),
    currentBuffer_(0),
    bufferStatus_(buffers_)
  {
    unsigned int index = 0;
    BufferVector::iterator first, last = bufferStatus_.end();
    for (first = bufferStatus_.begin(); first != last; ++first) {
      first->buffer = _bufferAddr[index];
      ++index;
    }
  }

  /** Deleting memory_ if it is its own_. */
  BufferManager::~BufferManager() 
  {
    cond_.broadcast();
    if (owner_)
      delete[] memory_;
  }

  /** 
   * Only a buffer of state FREE can be aquired for writing.
   * Its state is set to WRITE.
   *
   * @return The index of the acquired buffer. 
   */
  unsigned int 
  BufferManager::acquireNextWriteBuffer() throw (Miro::Exception)
  {
    Miro::Guard guard(mutex_);
    return protectedAcquireNextWriteBuffer();
  }

  /**
   * @param index The index of the acquired buffer. 
   * @param n The number of readers.
   */
  void 
  BufferManager::switchWrite2ReadBuffer(unsigned int _index, unsigned int _n) 
    throw (Miro::EOutOfBounds)
  {
    assert(_index < buffers_);
    Miro::Guard guard(mutex_);
    protectedSwitchWrite2ReadBuffer(_index, _n);
  }

  /**
   * A Buffer of state FREE or READ can be aquired for reading.
   * Its state is set to WRITE.
   *
   * @return The index of the acquired buffer. 
   */
  unsigned int 
  BufferManager::acquireCurrentReadBuffer() throw (Miro::Exception)
  {
    Miro::Guard guard(mutex_);
    return protectedAcquireCurrentReadBuffer();
  }
   
  /**
   * A Buffer of state FREE or READ can be aquired for reading.
   * Its state is set to WRITE.
   *
   * @return The index of the acquired buffer. 
   */
  unsigned int 
  BufferManager::acquireNextReadBuffer() throw (Miro::Exception)
  {
    Miro::Guard guard(mutex_);
    return protectedAcquireNextReadBuffer();
  }

  /** 
   * @param index The index of the acquired buffer. 
   */
  void
  BufferManager::releaseReadBuffer(unsigned int _index) 
    throw (Miro::EOutOfBounds)
  {
    assert(_index < buffers_);
    Miro::Guard guard(mutex_);
    protectedReleaseReadBuffer(_index);
  }

  // synchronised methods.

  unsigned int 
  BufferManager::protectedAcquireNextWriteBuffer() throw (Miro::Exception)
  {
    unsigned int index = currentBuffer_;
    do {
      ++index;
      index %= buffers_;
      if (bufferStatus_[index].state == BufferEntry::FREE) {
	bufferStatus_[index].state = BufferEntry::WRITING;
	return index;
      }
    } 
    while (index != currentBuffer_);

    throw Miro::Exception("No buffer available for writing.");
  }

  void 
  BufferManager::protectedSwitchWrite2ReadBuffer(unsigned int _index, unsigned int _n) 
    throw (Miro::EOutOfBounds)
  {
    if (bufferStatus_[_index].state == BufferEntry::WRITING) {
     bufferStatus_[_index].state = 
	(_n != 0)? BufferEntry::READING : BufferEntry::FREE;
      bufferStatus_[_index].readers = _n;
      currentBuffer_ = _index;
      cond_.broadcast();
    }
    else
      throw Miro::EOutOfBounds("Trying to free wrong buffer.");
  }

  unsigned int 
  BufferManager::protectedAcquireCurrentReadBuffer() throw (Miro::Exception)
  {
    if (bufferStatus_[currentBuffer_].state == BufferEntry::FREE ||
	bufferStatus_[currentBuffer_].state == BufferEntry::READING ) {
      bufferStatus_[currentBuffer_].state = BufferEntry::READING;
      ++bufferStatus_[currentBuffer_].readers;
      return currentBuffer_;
    }

    throw Miro::Exception("Current buffer not free.");
  }
   
  unsigned int 
  BufferManager::protectedAcquireNextReadBuffer() throw (Miro::Exception)
  {
    cond_.wait();
    if (bufferStatus_[currentBuffer_].state == BufferEntry::FREE ||
	bufferStatus_[currentBuffer_].state == BufferEntry::READING ) {
      ++bufferStatus_[currentBuffer_].readers;
      bufferStatus_[currentBuffer_].state = BufferEntry::READING;
      return currentBuffer_;
    }

    throw Miro::Exception("Next current buffer not free.");
  }

  void
  BufferManager::protectedReleaseReadBuffer(unsigned int _index) 
    throw (Miro::EOutOfBounds)
  {
    if (bufferStatus_[_index].readers == 1) {
      bufferStatus_[_index].readers = 0;
      bufferStatus_[_index].state = BufferEntry::FREE;
    }
    else if (bufferStatus_[_index].readers > 1) {
      --bufferStatus_[_index].readers;
    }
    else
      throw Miro::EOutOfBounds("Trying to free wrong buffer.");
  }
}
