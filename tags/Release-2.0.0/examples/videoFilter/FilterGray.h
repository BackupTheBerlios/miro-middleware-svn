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
#ifndef FilterGray_h
#define FilterGray_h

#include "Parameters.h"         // Parameter class.
#include "video/VideoFilter.h"  // Base class .

//! Converting an RGB image into a gray image.
/**
 * This is just a simple example filter. Better and faster
 * implementations are surely possible.
 */
class FilterGray : public Video::Filter
{
  //! Super class short-cut.
  typedef Video::Filter Super;

public:
  //! Initializing constructor.
  FilterGray(Miro::ImageFormatIDL const& _inputFormat);

  //! Factory method for parameter class.
  FILTER_PARAMETERS_FACTORY(FilterGray);

  //! Initialization based on the supplied parameters.
  void init(Miro::Server& _server, Video::FilterParameters const * _params);
  //! Do the work.
  void process();

protected:
  //! The size of the output image.
  unsigned int outputSize_;
  //! Pointer to the instances parameters.
  FilterGrayParameters const * params_;
  //! Sum of the red, green and blue weights.
  double sum_;
};

#endif // FilterGray_h
