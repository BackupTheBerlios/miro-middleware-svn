// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
