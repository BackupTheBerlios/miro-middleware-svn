// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoBrokerLink_h
#define VideoBrokerLink_h

#include "miro/Synch.h"

#include <vector>

namespace Video
{
  // forward declarations
  class Filter;

  class BrokerLink
  {
  public:
    BrokerLink(Miro::Mutex& _mutex,
	       Miro::Condition& _condition,
	       unsigned int& _index);

    void setIndex(unsigned int _index);

    bool protectedProcessed() const;
    unsigned int protectedIndex() const;

  protected:
    Miro::Mutex& mutex_;
    Miro::Condition& condition_;
    unsigned int& index_;
    bool processed_;
  };

  //! A std::vector of Video::BrokerLink pointes.
  typedef std::vector<BrokerLink *> BrokerLinkVector;

  struct BrokerRequest 
  {
    Filter * filter;
    BrokerLink * link;

    BrokerRequest();
    BrokerRequest(Filter * _filter, BrokerLink * _link);
  };

  typedef std::vector<BrokerRequest> BrokerRequestVector;

  inline
  BrokerLink::BrokerLink(Miro::Mutex& _mutex, 
			 Miro::Condition& _condition,
			 unsigned int& _index) :
    mutex_(_mutex),
    condition_(_condition),
    index_(_index),
    processed_(false)
  {}

  inline
  void
  BrokerLink::setIndex(unsigned int _index) {
    Miro::Guard guard(mutex_);
    processed_ = true;
    index_ = _index;
    condition_.broadcast();
  }

  inline
  bool
  BrokerLink::protectedProcessed() const {
    return processed_;
  }

  inline
  BrokerRequest::BrokerRequest() 
  {}

  inline
  BrokerRequest::BrokerRequest(Filter * _filter, BrokerLink * _link) :
    filter(_filter),
    link(_link)
  {}

}

#endif // VideoBrokerLink_h
