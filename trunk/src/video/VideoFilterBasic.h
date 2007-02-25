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
    FILTER_PARAMETERS_FACTORY(FilterCopy);

  protected:
    unsigned int imageSize_;
  };

  class FilterSwap3 : public Filter
  {
    typedef Filter Super;
  public:
    FilterSwap3(const Miro::ImageFormatIDL& _inputFormat);
    virtual void process();
    FILTER_PARAMETERS_FACTORY(FilterSwap3);

  protected:
    unsigned int imageSize_;
  };

  class FilterSwap4 : public Filter
  {
    typedef Filter Super;
  public:
    FilterSwap4(const Miro::ImageFormatIDL& _inputFormat);
    virtual void process();
    FILTER_PARAMETERS_FACTORY(FilterSwap4);

  protected:
    unsigned int imageSize_;
  };

  class FilterFlip : public Filter
  {
    typedef Filter Super;
  public:
    FilterFlip(const Miro::ImageFormatIDL& _inputFormat);
    virtual void process();
    FILTER_PARAMETERS_FACTORY(FilterFlip);

  protected:
    unsigned int rowSize_;
  };

  class FilterReverse : public Filter
  {
    typedef Filter Super;
  public:
    FilterReverse(const Miro::ImageFormatIDL& _inputFormat);
    virtual void process();
    FILTER_PARAMETERS_FACTORY(FilterReverse);

  protected:
    unsigned int rowSize_;
    int bytesPerPixel;
  };

  class FilterHalfImage : public Filter
  {
    typedef Filter Super;
  public:
    FilterHalfImage(const Miro::ImageFormatIDL& _inputFormat);

    FILTER_PARAMETERS_FACTORY(FilterHalfImage);

    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void process();

  protected:
    unsigned int rowSize_;
    unsigned int rowSize2_;
    unsigned int offset_;
  };
}

#endif
