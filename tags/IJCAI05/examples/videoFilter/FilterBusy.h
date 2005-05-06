// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FilterBusy_h
#define FilterBusy_h

#include "Parameters.h"
#include "video/VideoFilter.h"

class FilterBusy : public Video::Filter
{
  typedef Video::Filter Super;
public:
  FilterBusy(const Miro::ImageFormatIDL& _inputFormat);
  virtual void process();
  FILTER_PARAMETERS_FACTORY(FilterBusy);

protected:
  unsigned int imageSize_;
};

#endif // FilterBusy_h
