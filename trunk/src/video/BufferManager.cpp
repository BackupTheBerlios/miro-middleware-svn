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

#include "miro/Exception.h"
#include "miro/ExceptionC.h"

namespace Video
{
  BufferManager::BufferManager(unsigned int _buffers, 
			       unsigned int _bufferSize,
			       unsigned char * _memory) :
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

  BufferManager::BufferManager(unsigned int _buffers, 
			      unsigned char * _bufferAddr[], 
			      unsigned char * _memory) :
    buffers_(_buffers),
    bufferSize_(0),
    memory_(_memory),
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

  BufferManager::~BufferManager() 
  {
    cond_.broadcast();
    if (owner_)
      delete[] memory_;
  }

  unsigned int 
  BufferManager::acquireNextWriteBuffer()
  {
    Miro::Guard guard(mutex_);
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
  BufferManager::releaseWriteBuffer(unsigned int _index)
  {
    Miro::Guard guard(mutex_);
    if (bufferStatus_[_index].state == BufferEntry::WRITING) {
      bufferStatus_[_index].readers = 0;
      bufferStatus_[_index].state = BufferEntry::FREE;
      currentBuffer_ = _index;
      cond_.broadcast();
    }
    else
      throw Miro::EOutOfBounds("Trying to free wrong buffer.");
  }

  unsigned int 
  BufferManager::acquireCurrentReadBuffer()
  {
    Miro::Guard guard(mutex_);
    if (bufferStatus_[currentBuffer_].state == BufferEntry::FREE ||
	bufferStatus_[currentBuffer_].state == BufferEntry::READING ) {
      ++bufferStatus_[currentBuffer_].readers;
      bufferStatus_[currentBuffer_].state = BufferEntry::READING;
      return currentBuffer_;
    }

    throw Miro::Exception("Current buffer not free.");
  }
   
  unsigned int 
  BufferManager::acquireNextReadBuffer()
  {
    Miro::Guard guard(mutex_);
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
  BufferManager::releaseReadBuffer(unsigned int _index)
  {
    Miro::Guard guard(mutex_);
    if (bufferStatus_[_index].readers == 1) {
      bufferStatus_[_index].readers = 0;
      bufferStatus_[_index].state = BufferEntry::FREE;
    }
    else if (bufferStatus_[_index].readers > 1) {
      --bufferStatus_[_index].readers;
    }
  }
};
