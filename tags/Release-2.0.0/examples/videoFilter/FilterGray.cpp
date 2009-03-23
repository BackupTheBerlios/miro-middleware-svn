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
#include "FilterGray.h"       // Class declaration.
#include "miro/VideoHelper.h" // Global helper functions.
#include "miro/Exception.h"   // Exception declarations.


// Factory method implementation.
FILTER_PARAMETERS_FACTORY_IMPL(FilterGray);

/**
 * The filter expects the input format palette to be Miro::RGB_24. Otherwise
 * a Miro::Exception is thrown.
 */
FilterGray::FilterGray(Miro::ImageFormatIDL const& _inputFormat) :
  Super(_inputFormat)
{
  if (inputFormat_.palette != Miro::RGB_24)
    throw Miro::Exception("FilterGray: unsupported input format (RGB required).");
  outputFormat_.palette = Miro::GREY_8;

  outputSize_ = Miro::getImageSize(outputFormat_);
}

/**
 * Processing of the configuration file parameters.
 */
void
FilterGray::init(Miro::Server& _server, Video::FilterParameters const * _params)
{
  // Down cast the params pointer to the FilterGray parameters class.
  params_ = dynamic_cast<FilterGrayParameters const *>(_params);
  // Assert that the cast was successfull.
  assert(params_ != NULL);

  sum_ = params_->weightRed + params_->weightGreen + params_->weightBlue;

  // Super class initialization.
  Super::init(_server, _params);
}

/**
 * Image conversion.
 */
void
FilterGray::process()
{
  // Pointer to the input image.
  unsigned char const * src = inputBuffer();
  // Pointer to the output image.
  unsigned char * dest = outputBuffer();
  // Past the end pointer of the output image for termination condition.
  unsigned char * last = dest + outputSize_;

  while (dest < last) {
    *(dest++) = 
      (unsigned char) ( ( *(src++) * params_->weightRed +
			  *(src++) * params_->weightGreen +
			  *(src++) * params_->weightBlue) 
			/ sum_) ;
  }
}
