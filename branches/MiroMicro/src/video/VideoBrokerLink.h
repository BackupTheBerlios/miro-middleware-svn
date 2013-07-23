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
#ifndef VideoBrokerLink_h
#define VideoBrokerLink_h

#include "miro/Synch.h"

#include <vector>
#include <iostream>

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
    index_ = _index;
    processed_ = true;
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
