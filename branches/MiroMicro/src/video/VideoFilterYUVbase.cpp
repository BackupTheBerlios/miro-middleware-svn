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
#include "VideoFilterYUVbase.h"

#include <miro/Exception.h>

#include <iostream>
#include <algorithm>

namespace Video
{
  
  using std::max;
  using std::min;

  //---------------------------------------------------------------
  FilterYUVbase::FilterYUVbase(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    initTables();
  }

  //---------------------------------------------------------------
  FilterYUVbase::~FilterYUVbase()
  {
  }


  //---------------------------------------------------------------
  void 
  FilterYUVbase::initTables()
  {
    for (int i = 0; i < 256; i++) {
      for (int j = 0; j < 256; j++) {

	int r = i + (((j - 128) * 1434) / 2048);
	int b = i + (((j - 128) * 2078) / 2048);
	int g1 = (((i - 128) * 406) / 2048) + (((j - 128) * 595) / 2048);
	int g2 = i - j;
		
	t_r[(i << 8) | j] = std::min(std::max(0, r), 255);
	t_b[(i << 8) | j] = std::min(std::max(0, b), 255);
	t_g1[(i << 8) | j] = std::min(std::max(0, g1), 255);
	t_g2[(i << 8) | j] = std::min(std::max(0, g2), 255);
      }
    }
  }
};
