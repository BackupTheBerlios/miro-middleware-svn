// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Miro_TimeSeries_h
#define Miro_TimeSeries_h

#include <ace/Time_Value.h>

namespace Miro
{
  struct TimeStats
  {
    ACE_Time_Value min;
    ACE_Time_Value max;
    ACE_Time_Value mean;
    ACE_Time_Value var;
  };

  template<unsigned int N = 100UL>
  class TimeSeries
  {
  public:
    static const unsigned int SIZE = N;

    TimeSeries();

    void start(const ACE_Time_Value& _stamp = ACE_OS::gettimeofday());
    void stop(const ACE_Time_Value& _stamp = ACE_OS::gettimeofday());

    void eval(TimeStats& _stats) const;
    unsigned int size() const;
    bool full() const;

  protected:
    ACE_Time_Value stamp_[SIZE];
    ACE_Time_Value * begin_;
    ACE_Time_Value * end_;
    ACE_Time_Value * next_;
    ACE_Time_Value start_;

    bool full_;
    bool started_;

    static ACE_Time_Value tSquare(const ACE_Time_Value& _t);
  };

  template<unsigned int N>
  TimeSeries<N>::TimeSeries() :
    begin_(stamp_),
    end_(stamp_ + SIZE),
    next_(stamp_),
    full_(false),
    started_(false)
  {
  }

  template<unsigned int N>
  bool
  TimeSeries<N>::full() const
  {
    return full_;
  }

  template<unsigned int N>
  unsigned int
  TimeSeries<N>::size() const 
  {
    return (full_)? SIZE : next_ - begin_;
  }

  template<unsigned int N>
  void
  TimeSeries<N>::start(const ACE_Time_Value& _stamp)
  {
    start_ = _stamp;
    started_ = true;
  }

  template<unsigned int N>
  void
  TimeSeries<N>::stop(const ACE_Time_Value& _stamp)
  {
    // prevent nonsense
    assert(started_);

    *next_++ = _stamp - start_;
    started_ = false;

    if (next_ == end_) {
      next_ = begin_;
      full_ = true;
    }
  }

  template<unsigned int N>
  void
  TimeSeries<N>::eval(TimeStats& _stats) const
  {
    ACE_Time_Value * first = begin_;
    ACE_Time_Value * last = (full_)? end_ : next_;

    if (last - first > 2) {
      ACE_Time_Value * pMin = begin_;
      ACE_Time_Value * pMax = begin_;
      
      _stats.mean = *begin_;
      _stats.var = tSquare(*begin_);
      for (++first; first != last; ++first) {
	_stats.mean += *first;
	_stats.var += tSquare(*first);
	if (*first > *pMax) {
	  pMax = first;
	}
	else if (*first < *pMin) {
	  pMin = first;
	}
      }
      _stats.min = *pMin;
      _stats.max = *pMax;
      _stats.mean *= 1. / (double)size();
      _stats.var -= tSquare(_stats.mean);
      _stats.var *= 1. / ((double)(size() - 1));
    }
    else {
      _stats.min = 0;
      _stats.max = 0;
      _stats.mean = 0;
      _stats.var = 0;
    }
  }

  template<unsigned int N>
  ACE_Time_Value
  TimeSeries<N>::tSquare(const ACE_Time_Value& _t) {
    ACE_Time_Value v;
    double t = (double)_t.sec() + (double)_t.usec() / 1000000.;
    t *= t;
    v.set(t);
    return v;
  }
}

#endif /* Miro_TimeSeries_h */
