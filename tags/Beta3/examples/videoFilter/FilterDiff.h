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
#ifndef FilterDiff_h
#define FilterDiff_h

#include "Parameters.h"         // Parameter class.
#include "video/VideoFilter.h"  // Base class .

#include <deque>

//! Taking the difference of two images.
/**
 * This is just a simple example filter. Better and faster
 * implementations are surely possible.
 */
class FilterDiff : public Video::Filter
{
  //! Super class short-cut.
  typedef Video::Filter Super;

public:
  //! Initializing constructor.
  FilterDiff(Miro::ImageFormatIDL const& _inputFormat);

  //! Factory method for parameter class.
  FILTER_PARAMETERS_FACTORY(FilterDiff);

  //! Initialization based on the supplied parameters.
  void init(Miro::Server& _server, Video::FilterParameters const * _params);

  //! Do the work.
  void process();

protected:
  // internal types
  struct FilterLock
  {
    unsigned long index;
    unsigned char const * buffer;

    FilterLock() {}
    FilterLock(unsigned long _i, unsigned char const * _b) : index(_i), buffer(_b) {}
  };
  typedef std::deque<FilterLock> FilterQueue;

  //! The size of the output image.
  unsigned int outputSize_;
  //! Pointer to the instances parameters.
  FilterDiffParameters const * params_;
  //! Queue of locked images.
  FilterQueue queue_;
};

#endif // FilterDiff_h
