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

/*! \file VideoFilterYUVbase.cpp
 *  \brief Image conversion routines YUV->RGB
 */

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
