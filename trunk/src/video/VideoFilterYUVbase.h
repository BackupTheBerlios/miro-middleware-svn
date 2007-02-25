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
#ifndef VideoFilterYUVbase_h
#define VideoFilterYUVbase_h

/*! \file VideoFilterYUVbase.h
 *  \brief Image conversion routines YUV->RGB (interface)
 */

#include "VideoFilter.h"

namespace Video
{
    //! Lookup table driven conversion from YUV422 to RGB
    class FilterYUVbase : public Filter
    {
      typedef Filter Super;
    public:
        FilterYUVbase(Miro::ImageFormatIDL const& _inputFormat);
	virtual ~FilterYUVbase();
	
    protected:
        void initTables();
			
        unsigned char t_r[65536];
	unsigned char t_b[65536];
	unsigned char t_g1[65536];
	unsigned char t_g2[65536];
    };
};

#endif // VideoFilterYUVbase_h

