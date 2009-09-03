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
#ifndef VideoFilterYUV411toYUV_h
#define VideoFilterYUV411toYUV_h

/*! \file VideoFilterYUV411toYUV.h
 *  \brief Image conversion routines YUV411->YUV (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilter.h"

namespace Video
{
  //! Lookup table driven conversion from YUV411 to YUV
  class FilterYUV411toYUV : public Filter
  {
    typedef Filter Super;
  public:
    FilterYUV411toYUV(Miro::ImageFormatIDL const& _inputFormat);
    virtual ~FilterYUV411toYUV();
    virtual void process();
  };
};

#endif // VideoFilterYUV411toYUV_h
