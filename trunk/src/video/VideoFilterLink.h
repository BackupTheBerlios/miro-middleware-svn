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
#ifndef FilterLink_h
#define FilterLink_h

#include <vector>

namespace Video
{
  // forward declarations
  class Filter;
  class FilterImageParameters;

  class FilterPreLink
  {
  public:
    FilterPreLink(Filter * _filter);

    Filter const * const filter() const;
    Filter * const filter();
    void id(unsigned long _id);
    unsigned long id() const;
    void buffer(unsigned long _index, 
		unsigned char const * _buffer,
		FilterImageParameters const * _params);
    unsigned char const * buffer() const;
    FilterImageParameters const * params() const;
    unsigned long index() const;
    bool bufferAvailable() const;

  protected:
    Filter * filter_;
    unsigned long id_;
    unsigned long index_;
    unsigned char const * buffer_;
    FilterImageParameters const * params_;
  };

  //! A std::vector of Video::FilterPreLink objects.
  typedef std::vector<FilterPreLink> PreLinkVector;  

  class FilterSuccLink
  {
  public:
    FilterSuccLink(Filter * _filter, unsigned long _index);

    Filter const * const filter() const;
    Filter * const filter();
    unsigned long index() const;

  protected:
    Filter * filter_;
    unsigned long index_;
  };

  inline
  FilterPreLink::FilterPreLink(Filter * _filter) :
    filter_(_filter),
    buffer_(NULL)
  {}

  inline
  Filter const * const
  FilterPreLink::filter() const {
    return filter_;
  }

  inline
  Filter * const
  FilterPreLink::filter() {
    return filter_;
  }

  inline
  void
  FilterPreLink::id(unsigned long _id) {
    id_ = _id;
  }

  inline
  unsigned long
  FilterPreLink::id() const {
    return id_;
  }

  inline
  void 
  FilterPreLink::buffer(unsigned long _index, 
			unsigned char const * _buffer,
			FilterImageParameters const * _params)
  {
    index_ = _index;
    buffer_ = _buffer;
    params_ = _params;
  }

  inline
  unsigned char const *
  FilterPreLink::buffer() const {
    return buffer_;
  }

  inline
  FilterImageParameters const *
  FilterPreLink::params() const {
    return params_;
  }

  inline
  unsigned long
  FilterPreLink::index() const {
    return index_;
  }

  inline
  bool
  FilterPreLink::bufferAvailable() const {
    return (buffer_ != NULL);
  }

  inline
  FilterSuccLink::FilterSuccLink(Filter * _filter, unsigned long _index) :
    filter_(_filter),
    index_(_index)
  {}

  inline
  Filter const * const
  FilterSuccLink::filter() const {
    return filter_;
  }

  inline
  Filter * const
  FilterSuccLink::filter() {
    return filter_;
  }

  inline
  unsigned long
  FilterSuccLink::index() const {
    return index_;
  }
}

#endif // FilterLink_h
