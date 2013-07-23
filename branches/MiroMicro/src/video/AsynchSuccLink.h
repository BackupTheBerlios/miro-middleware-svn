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
