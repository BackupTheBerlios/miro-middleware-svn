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

#include "AsynchPreLink.h"
#include "VideoFilter.h"
#include "VideoFilterLink.h"
#include "BufferManager.h"

#include "miro/Log.h"

namespace Video
{
  void
  AsynchPreLink::releaseBuffer() 
  {
    if (buffer_) {
      filter_->bufferManager()->releaseReadBuffer(index_);
      // set the buffer to be no longer available
      buffer_ = NULL;
    }
  }

  void 
  AsynchPreLink::acquireBuffer() 
  {
    filter_->bufferManager()->acquireReadBuffer(index_);
  }

  void 
  AsynchPreLink::setBufferPreLink(PreLinkVector& _filterVector) const 
  {
    _filterVector[localIndex_].buffer(index_, buffer_, params_);
  }
}
