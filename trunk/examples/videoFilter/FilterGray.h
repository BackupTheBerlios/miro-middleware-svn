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

#include "Parameters.h"
#include "video/VideoFilter.h"

class FilterGray : public Video::Filter
{
  typedef Video::Filter Super;

public:
  FilterGray(Miro::ImageFormatIDL const& _inputFormat);

  FILTER_PARAMETERS_FACTORY(FilterGray);

  void init(Video::FilterParameters const * _params);
  void process();

protected:
  unsigned int outputSize_;
  FilterGrayParameters const * params_;
  double sum_;
};

#endif // FilterGray_h
