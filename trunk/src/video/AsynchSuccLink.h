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
#ifndef Video_AsynchSuccLink_h
#define Video_AsynchSuccLink_h

#include "AsynchBufferSet.h"

// forward declarations
class ACE_Time_Value;

namespace Video
{
  // forward declarations
  class FilterImageParameters;

  //! Link to an asynchronous successor filter.
  /**
   * The successor link point to another device tree. This gets a
   * bit tricky with all the subscription management and
   * so on, but we work hard on it. Actually it is needed for
   * stereo vision as well as for asynchronous low priority processing.
   */
  class AsynchSuccLink
  {
  public:
    //! Initializing constructor.
    AsynchSuccLink(AsynchBufferSet * _bufferSet, unsigned long _id);

    //! Set the buffer for processing.
    /** The buffer is assumed to be already acquired for read access. */
    void buffer(ACE_Time_Value const& _imageTime,
		unsigned long _index, 
		unsigned char const * _buffer,
		FilterImageParameters const * _params) throw();

    bool protectedCalcConnectivity() throw();

  protected:
    //! Pointer to the buffer set, that manages the asynchronous connection.
    AsynchBufferSet * bufferSet_;
    //! Id (index) in the buffer set.
    unsigned long id_;
    //! Flag indicating connectedness.
    bool connected_;
  };

  inline
  AsynchSuccLink::AsynchSuccLink(AsynchBufferSet * _bufferSet, 
				 unsigned long _id) :
    bufferSet_(_bufferSet),
    id_(_id),
    connected_(false)
  {}

  inline
  bool
  AsynchSuccLink::protectedCalcConnectivity() throw() {
    connected_ = bufferSet_->protectedCalcConnectivity();
    return connected_;
  }
}

#endif // Video_AsynchSuccLink_h
