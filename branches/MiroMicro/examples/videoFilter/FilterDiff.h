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
