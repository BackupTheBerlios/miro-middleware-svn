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
#include "VideoBrokerLink.h"
#include "VideoImpl.h"
#include "VideoDevice.h"
#include "BufferManager.h"

#include "miro/TimeHelper.h"

namespace Miro
{
  // Implementation skeleton constructor
  VideoBrokerImpl::VideoBrokerImpl(::Video::Device * _device) :
    device_(_device)
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
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    if (connections.length() == 0)
      throw EOutOfBounds("Empty connection set.");
    buffers = new BufferSetIDL;
    buffers->length(connections.length());

    // locking harness
    Mutex mutex;
    Condition cond(mutex);

    Guard guard(mutex);
    
    ::Video::BrokerRequestVector request;
	
    std::cout << "process all filters" << std::endl;

    for (unsigned int i = 0; i < connections.length(); ++i) {
      std::cout << "find corresponding filter" << std::endl;
      std::string name = (char const *)connections[i].filter;
      ::Video::Filter * filter = device_->findByInterfaceName(name);

      if (filter == NULL) {
	::Video::BrokerRequestVector::const_iterator first, last = request.end();
	for (first = request.begin(); first != last; ++first) {
	  delete first->link;
	}
	throw EOutOfBounds("Unknown filter name.");
      }

      assert(filter->interface() != NULL);
      filter->interface()->checkClientId(connections[i].id);

      std::cout << "install a callback for each" << std::endl;
      ::Video::BrokerLink * link = new ::Video::BrokerLink(mutex, cond, buffers[i]);
      request.push_back(::Video::BrokerRequest(filter, link));
    }

    ::Video::BrokerRequestVector::const_iterator first, last = request.end();
    for (first = request.begin(); first != last; ++first) {
      device_->enqueueBrokerRequest(*first);
    }

    std::cout << "wait for completion of all filters" << std::endl;
    ACE_Time_Value maxWait = ACE_OS::gettimeofday() + ACE_Time_Value(10, 100000);
    bool buffersPending;
    do {
      if (cond.wait(&maxWait) == -1) {
	// This will leak the broker links!
	// Even worse, the processed once will be dangling
	throw ETimeOut();
      }

      buffersPending = false;
      for (first = request.begin(); first != last; ++first) {
	if (!first->link->protectedProcessed()) {
	  buffersPending = true;
	  break;
	}
      }
    }
    while (buffersPending);


    std::cout << "get time stamp for images" << std::endl;
    ACE_Time_Value t;
    CORBA::ULong index = 0;
    first = request.begin();
    t = first->filter->
      interface()->bufferManager()->bufferTimeStamp(buffers[index]);
    std::cout << "check for filter synchronisation: " << t << std::endl;
    for (++index, ++first; first != last; ++first, ++index) {
      std::cout << "t: " << first->filter->
	interface()->bufferManager()->bufferTimeStamp(buffers[index]) << std::endl;
      assert(first->filter->
	     interface()->bufferManager()->bufferTimeStamp(buffers[index])
	     == t);
      
    }
    TimeIDL stamp;
    timeA2C(t, stamp);

    // clean up filter links
    for (first = request.begin(); first != last; ++first) {
      delete first->link;
    }

    std::cout << __PRETTY_FUNCTION__  << " end" << std::endl;


    return stamp;
  }
  
  void 
  VideoBrokerImpl::releaseImageSet(const ConnectionSetIDL & connections,
				   const BufferSetIDL & buffers)
    ACE_THROW_SPEC ((EOutOfBounds))
  {
    for (CORBA::ULong i = 0; i < connections.length(); ++i) {
      // find corresponding filter
      std::string name = (char const *)connections[i].filter;
      ::Video::Filter * filter = device_->findByInterfaceName(name);

      if (filter == NULL) {
	throw EOutOfBounds("Unknown filter name.");
      }

      assert(filter->interface() != NULL);
      // check for id
      filter->interface()->checkClientId(connections[i].id);
      // release buffer
      filter->interface()->bufferManager()->releaseReadBuffer(buffers[i]);
    }
  }
  
  FilterTreeIDL * 
  VideoBrokerImpl::filterTreeStats() ACE_THROW_SPEC (())
  {
    FilterTreeIDL * tree = new FilterTreeIDL();

    device_->filterTreeStats(*tree);

    return tree;
  }
}
