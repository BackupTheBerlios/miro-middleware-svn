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
