// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Video_BufferManager_h
#define Video_BufferManager_h

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/Log.h"
#include "miro/Parameters.h"
#include "idl/ExceptionC.h"

#include <vector>
#include <exception>

namespace Video
{
  // forward declarations
  class Filter;
  class FilterImageParameters;

  //! Class for managing an array of memory buffers.
  class BufferManager
  {
  public:
    //! Initializing constructor.
    BufferManager(Filter const * const _filter,
		  unsigned int _buffers, 
		  unsigned int _bufferSize, 
		  unsigned char * _memory = NULL) throw (std::bad_alloc);
    //! Initializing constructor.
    BufferManager(Filter const * const _filter,
		  unsigned int _buffers,
		  unsigned char * _bufferAddr[]);
    //! Cleaning up.
    virtual ~BufferManager();

    //! Number of managed buffers.
    unsigned int buffers() const throw ();
    //! Address of a buffer.
    /** @param index The index of the buffer in the buffer array. */
    unsigned char * bufferAddr(unsigned int _index) throw ();
    //! The time stamp of a buffer.
    /** @param index The index of the buffer in the buffer array. */
    const ACE_Time_Value& bufferTimeStamp(unsigned int _index) const throw ();
    //! Set the time stamp of a buffer.
    /** @param index The index of the buffer in the buffer array. */
    void bufferTimeStamp(unsigned int _index, ACE_Time_Value const& _stamp) throw ();
    //! Parameters of the filtered image.
    /** @param index The index of the image in the buffer array. */
    FilterImageParameters * imageParameters(unsigned int _index) throw ();

    //! Acquire a buffer for writing.
    unsigned int acquireNextWriteBuffer() throw (Miro::Exception);
    //! Relable a write buffer into a read buffer with n readers
    void switchWrite2ReadBuffer(unsigned int _index, unsigned int _n) 
      throw (Miro::EOutOfBounds);

    //! Acquire the current buffer for reading.
    unsigned int acquireCurrentReadBuffer() throw (Miro::Exception);
    //! Acquire the next written buffer for reading.
    unsigned int acquireNextReadBuffer() throw (Miro::Exception);
    //! Acquire a buffer one more time for reading.
    /** The buffer needs to be already acquired. This just increases the read count. */
    void acquireReadBuffer(unsigned int _index) throw (Miro::EOutOfBounds);
    //! Release a buffer that was aquired for reading.
    void releaseReadBuffer(unsigned int _index) throw (Miro::EOutOfBounds);
    
  protected:
    //! Acquire a buffer for writing.
    virtual unsigned int protectedAcquireNextWriteBuffer() 
      throw (Miro::Exception);

    //! Relable a write buffer into a read buffer with n readers
    virtual void protectedSwitchWrite2ReadBuffer(unsigned int _index, 
						 unsigned int _n) 
      throw (Miro::EOutOfBounds);

    //! Acquire the current buffer for reading.
    virtual unsigned int protectedAcquireCurrentReadBuffer() 
      throw (Miro::Exception);
    //! Acquire the next written buffer for reading.
    virtual unsigned int protectedAcquireNextReadBuffer()
      throw (Miro::Exception);
    //! Acquire the specified buffer for reading.
    virtual void protectedAcquireReadBuffer(unsigned int _index) 
      throw (Miro::EOutOfBounds);
    //! Release a buffer that was aquired for reading.
    virtual void protectedReleaseReadBuffer(unsigned int _index) 
      throw (Miro::EOutOfBounds);
    
    //! Struct for managing entries of a image array buffer.
    struct BufferEntry
    {
      //! State of the buffer.
      enum State {FREE, WRITING, READING};

      //! Time stamp of the entry.
      ACE_Time_Value time;
      //! State of the entry.
      State state;
      //! Number of reading clients.
      unsigned int readers;
      //! Pointer to the manged buffer.
      unsigned char * buffer;
      //! Pointer to the filter image parameters.
      FilterImageParameters * params;

      //! Default constructor.
      /** 
       * By default, the state of a buffer entry is free and it has 0
       * readers. 
       */
      BufferEntry() throw () : state(FREE), readers(0) {}
    };

    typedef std::vector<BufferEntry> BufferVector;

    //! Number of buffers.
    unsigned int buffers_;
    //! Size of each buffer.
    unsigned int bufferSize_;
    //! The memory used to store the buffers.
    unsigned char * memory_;
    //! If true, the instance will delete the memory_ on destruction.
    bool owner_;

    //! For thread safety.
    Miro::Mutex mutex_;
    //! For waiting on the next buffer.
    Miro::Condition cond_;
    //! The index of the current buffer.
    unsigned int currentBuffer_;
    //! The status of each managed buffer in the buffer array.
    BufferVector bufferStatus_;
    //! Pointer to the camera parameters of this device
    const Miro::CameraParameters * camParams_;
  };

  inline
  unsigned int
  BufferManager::buffers() const throw () {
    return buffers_;
  }

  inline 
  unsigned char * 
  BufferManager::bufferAddr(unsigned int _index) throw () {
    MIRO_ASSERT(_index < buffers_);
    return bufferStatus_[_index].buffer;
  }
  inline 
  const ACE_Time_Value&
  BufferManager::bufferTimeStamp(unsigned int _index) const throw () {
    MIRO_ASSERT(_index < buffers_);
    return bufferStatus_[_index].time;
  }
  inline 
  void
  BufferManager::bufferTimeStamp(unsigned int _index, 
				 ACE_Time_Value const& _stamp) throw () {
    MIRO_ASSERT(_index < buffers_);
    bufferStatus_[_index].time = _stamp;
  }
  inline 
  FilterImageParameters * 
  BufferManager::imageParameters(unsigned int _index) throw () {
    MIRO_ASSERT(_index < buffers_);
    return bufferStatus_[_index].params;
  }
}

#endif // Video_BufferManager_h
