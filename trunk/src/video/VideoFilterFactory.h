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
