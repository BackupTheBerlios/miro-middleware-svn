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
#ifndef Video_FilterBasic_h
#define Video_FilterBasic_h

#include "VideoFilter.h"

namespace Video
{
  class FilterCopy : public Filter
  {
    typedef Filter Super;
  public:
    FilterCopy(const Miro::ImageFormatIDL& _inputFormat);
    virtual void process();
  protected:
    unsigned int imageSize_;
  };

  class FilterSwap3 : public Filter
  {
    typedef Filter Super;
  public:
    FilterSwap3(const Miro::ImageFormatIDL& _inputFormat);
    virtual void process();
  protected:
    unsigned int imageSize_;
  };

  class FilterSwap4 : public Filter
  {
    typedef Filter Super;
  public:
    FilterSwap4(const Miro::ImageFormatIDL& _inputFormat);
    virtual void process();
  protected:
    unsigned int imageSize_;
  };

  class FilterFlip : public Filter
  {
    typedef Filter Super;
  public:
    FilterFlip(const Miro::ImageFormatIDL& _inputFormat);
    virtual void process();
  protected:
    unsigned int rowSize_;
  };

  class FilterHalfImage : public Filter
  {
    typedef Filter Super;
  public:
    FilterHalfImage(const Miro::ImageFormatIDL& _inputFormat);

    FILTER_PARAMETERS_FACTORY(FilterHalfImage);

    virtual void init(FilterParameters const *);
    virtual void process();
  protected:
    FilterHalfImageParameters const * params_;

    unsigned int rowSize_;
    unsigned int rowSize2_;
    unsigned int offset_;
  };
};

#endif
