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
#ifndef VideoAsynchPreLink_h
#define VideoAsynchPreLink_h

#include "VideoFilterLink.h"

#include <vector>

namespace Video
{
  // forward declarations
  class Filter;

  //! Link to an asynchronous predecessor filter.
  /**
   * The predecessor link points to another device tree. This gets a
   * bit tricky with all the subscription management and
   * so on, but we work hard on it. Actually it is needed for
   * stereo vision as well as for asynchronous low priority processing.
   */
  class AsynchPreLink
  {
  public:
    //! Initializing constructor.
    AsynchPreLink(Filter * _filter, unsigned int _localIndex);

    //! Accessor to the linked filter.
    Filter * filter();

    //! Set the buffer for processing.
    /** The buffer is assumed to be already acquired for read access. */
    void buffer(unsigned long _index, 
		unsigned char const * _buffer,
		FilterImageParameters const * _params) throw();
    //! Test if a buffer is already set.
    bool bufferAvailable() const throw();
    //! Release the buffer.
    void releaseBuffer();
    //! Acquire another reader for the buffer.
    void acquireBuffer();
    //! Set the buffer in the prelink list, that's used by the filter.
    /**
     * The actual filter implementation does not need to distinguish
     * between synchronous and asynchronous links.
     */ 
    void setBufferPreLink(PreLinkVector& _filterVector) const;

  protected:
    //! Pointer to the asynch successor filter.
    Filter * filter_;
    //! Index of the filter in the local successor link vector.
    unsigned long localIndex_;
    //! Index of the buffer in the buffer manager for the filter.
    unsigned long index_;
    //! Address of the filter.
    unsigned char const * buffer_;
    //! Address of the filter parameters.
    FilterImageParameters const * params_;
  };

  typedef std::vector<AsynchPreLink> AsynchPreLinkVector;

  inline
  AsynchPreLink::AsynchPreLink(Filter * _filter, 
			       unsigned int _localIndex) :
    filter_(_filter),
    localIndex_(_localIndex),
    index_(0),
    buffer_(NULL),
    params_(NULL)
  {}

  inline
  Filter * 
  AsynchPreLink::filter() {
    return filter_;
  }

  inline
  void 
  AsynchPreLink::buffer(unsigned long _index, 
			unsigned char const * _buffer,
			FilterImageParameters const * _params) throw() {
    index_ = _index;
    buffer_ = _buffer;
    params_ = _params;
  }

  inline
  bool
  AsynchPreLink::bufferAvailable() const throw() {
    return (buffer_ != NULL);
  }
}

#endif // VideoAsynchPreLink_h