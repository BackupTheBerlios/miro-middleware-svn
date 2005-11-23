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
#ifndef VideoFilterFactory_h
#define VideoFilterFactory_h

#include "idl/VideoC.h"

namespace Video
{
  class Filter;

  class FilterFactoryBase
  {
  public:
    virtual ~FilterFactoryBase();
    virtual Filter * instance(Miro::ImageFormatIDL const & _inputFormat) const = 0;
  };

  template<class T>
  class FilterFactory : public FilterFactoryBase
  {
  public:
    virtual Filter * instance(Miro::ImageFormatIDL const & _inputFormat) const;
  };

  template<class T>
  Filter *
  FilterFactory<T>::instance(Miro::ImageFormatIDL const & _inputFormat) const {
    return new T(_inputFormat);
  }
}

#endif
