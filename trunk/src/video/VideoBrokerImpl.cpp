// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "VideoBrokerImpl.h"
#include "VideoFilter.h"

#include "miro/TimeHelper.h"

namespace Miro
{
  // Implementation skeleton constructor
  VideoBrokerImpl::VideoBrokerImpl(::Video::Filter * _filter) :
    filter_(_filter)
  {
  }
  
  // Implementation skeleton destructor
  VideoBrokerImpl::~VideoBrokerImpl()
  {
  }

  TimeIDL 
  VideoBrokerImpl::acquireNextImageSet(const ConnectionSetIDL & connections,
				       BufferSetIDL_out buffers)
    ACE_THROW_SPEC ((EOutOfBounds, EDevIO, ETimeOut))
  {
    if (connections.length() == 0)
      throw EOutOfBounds("Empty connection set.");
    buffers->length(connections.length());

    // locking harness
    Mutex mutex;
    Condition cond(mutex);
    Guard guard(mutex);
	
    // process all filters
    ::Video::Filter * filter = NULL;
    for (unsigned int i = 0; i < connections.length(); ++i) {
      // find corresponding filter
      std::string name = (char const *)connections[i].filter;
      filter = filter_->findByName(name);

      if (filter == NULL)
	throw EOutOfBounds("Unknown filter name.");

      // install a callback for each
      //     filter->getForNextImage(mutex, cond, connections[i].id, buffer, processed);
    }

    // wait for completion of all filters
    ACE_Time_Value maxWait = ACE_OS::gettimeofday() + ACE_Time_Value(0, 100000);
    bool buffersPending;
    do {
      if (cond.wait(&maxWait) == -1)
	throw ETimeOut();
      buffersPending = false;
      for (unsigned int i = 0; i != buffers->length(); ++i) {
	if (buffers[i] == 0) {  // FIXME: need to verify buffer
	  buffersPending = true;
	  break;
	}
      }
    } while (buffersPending);

    // get time stamp for images
    TimeIDL t;
    //     timeA2C(filter->timeStamp(), t);
    return t;
  }
  
  void 
  VideoBrokerImpl::releaseImageSet(const ConnectionSetIDL & /* connections */,
				   const BufferSetIDL & /* buffers */)
    ACE_THROW_SPEC ((EOutOfBounds))
  {
    //Add your implementation here
  }
  
  FilterTreeIDL * 
  VideoBrokerImpl::filterTreeStats() ACE_THROW_SPEC (())
  {
    //Add your implementation here
    FilterTreeIDL * tree = new FilterTreeIDL();

    filter_->filterTreeStats(*tree);

    return tree;
  }
}
