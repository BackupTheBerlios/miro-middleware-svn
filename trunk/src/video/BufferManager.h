// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Video_BufferManager_h
#define Video_BufferManager_h

#include "miro/Synch.h"

#include <vector>

namespace Video
{
  struct BufferEntry
  {
    enum State {FREE, WRITING, READING};

    ACE_Time_Value time;
    State state;
    unsigned int readers;
    unsigned char * buffer;

    BufferEntry() : state(FREE), readers(0) {}
  };

  class BufferManager
  {
  public:
    BufferManager(unsigned int _buffers, 
		  unsigned int _bufferSize, 
		  unsigned char * _memory = NULL);
    BufferManager(unsigned int _buffers,
		  unsigned char * _bufferAddr[], 
		  unsigned char * _memory);
    ~BufferManager();

    unsigned int buffers() const;
    unsigned char * bufferAddr(unsigned int _index);
    const ACE_Time_Value& bufferTimeStamp(unsigned int _index) const;
    void bufferTimeStamp(unsigned int _index, ACE_Time_Value const& _stamp);

    unsigned int acquireNextWriteBuffer();
    void releaseWriteBuffer(unsigned int _index);

    unsigned int acquireCurrentReadBuffer();
    unsigned int acquireNextReadBuffer();
    void releaseReadBuffer(unsigned int _index);
    
  protected:
    typedef std::vector<BufferEntry> BufferVector;

    unsigned int buffers_;
    unsigned int bufferSize_;
    unsigned char * memory_;
    bool owner_;

    Miro::Mutex mutex_;
    Miro::Condition cond_;
    unsigned int currentBuffer_;
    BufferVector bufferStatus_;
  };

  inline
  unsigned int
  BufferManager::buffers() const {
    return buffers_;
  }

  inline 
  unsigned char * 
  BufferManager::bufferAddr(unsigned int _index) {
    return bufferStatus_[_index].buffer;
  }
  inline 
  const ACE_Time_Value&
  BufferManager::bufferTimeStamp(unsigned int _index) const {
    return bufferStatus_[_index].time;
  }
  inline 
  void
  BufferManager::bufferTimeStamp(unsigned int _index, ACE_Time_Value const& _stamp){
    bufferStatus_[_index].time = _stamp;
  }
};

#endif // Video_BufferManager_h
