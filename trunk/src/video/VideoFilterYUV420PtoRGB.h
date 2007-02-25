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
#ifndef VideoFilterYUV420PtoRGB_h
#define VideoFilterYUV420PtoRGB_h

/*! \file VideoFilterYUV420PtoRGB.h
 *  \brief Image conversion routines YUV411->RGB (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilter.h"

namespace Video
{

    //! Lookup table driven conversion from YUV411 to RGB
    class FilterYUV420PtoRGB : public Filter
    {
      typedef Filter Super;
      static long int crv_tab[256];
      static long int cbu_tab[256];
      static long int cgu_tab[256];
      static long int cgv_tab[256];
      static long int tab_76309[256];
      static unsigned char clp[1024];
      void InitConvtTbl();

    public:
        FilterYUV420PtoRGB(Miro::ImageFormatIDL const& _inputFormat);
	virtual ~FilterYUV420PtoRGB();

	virtual void process();
    };
};

#endif // VideoFilterYUV420PtoRGB_h

