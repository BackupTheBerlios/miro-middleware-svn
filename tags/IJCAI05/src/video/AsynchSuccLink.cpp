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

#include "AsynchSuccLink.h"
#include "AsynchBufferSet.h"

namespace Video
{
  // This method can not be inlined due to cyclic dependencies...
  void 
  AsynchSuccLink::buffer(ACE_Time_Value const& _imageTime,
			 unsigned long _index, 
			 unsigned char const * _buffer,
			 FilterImageParameters const * _params) throw() 
  {
    if (connected_)
      bufferSet_->buffer(id_, _imageTime, _index, _buffer, _params);
  }
}
