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
