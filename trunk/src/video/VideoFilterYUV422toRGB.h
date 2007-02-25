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
#ifndef VideoFilterYUV422toRGB_h
#define VideoFilterYUV422toRGB_h

/*! \file FilterYUV422toRGB.h
 *  \brief Image conversion routines YUV422->RGB (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilterYUVbase.h"

namespace Video
{
  //! Lookup table driven conversion from YUV422 to RGB
  class FilterYUV422toRGB : public FilterYUVbase
  {
    typedef FilterYUVbase Super;
  public:
    FilterYUV422toRGB(Miro::ImageFormatIDL const& _inputFormat);
    virtual ~FilterYUV422toRGB();
    virtual void process();
    FILTER_PARAMETERS_FACTORY(FilterYUV422toRGB);
  };
}

#endif // VideoFilterYUV422toRGB_h

